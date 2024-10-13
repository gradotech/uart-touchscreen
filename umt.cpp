#include "umt.h"
#include "umt_uart.h"
#include "umt_uinput.h"
#include "umt_packet.h"
#include "umt_queue.h"
#include "umt_watchdog.h"

#include <fcntl.h>

#include <thread>

#define UMT_UART_DEVICE      "/dev/ttyAMA0"
#define UMT_UINPUT_DEVICE    "/dev/uinput"
#define UMT_WD_TIMEOUT       50 /* ms */

static const bool enable_watchdog = true;

void umt_producer(UMTQueue<UMTPacket>& packetQueue)
{
	UARTManager *uartm = new UARTManager(UMT_UART_DEVICE, O_RDWR | O_NOCTTY);
	int ret;

	while (1) {
		UMTPacket packet;

		ret = uartm->getPacket(packet);
		if (ret) {
			UMT_LOG(UMT_DBG_LOG, "getPacked failed, ret %d\n", ret);
			continue;
		}

		packetQueue.enqueue(packet);
	}
}

void umt_consumer(UMTQueue<UMTPacket>& packetQueue)
{
	UINPUTManager uinputm(UMT_UINPUT_DEVICE, O_WRONLY | O_NONBLOCK);
	UMTWatchdog watchdog(enable_watchdog, UMT_WD_TIMEOUT);
	bool ret;

	while (1) {
		UMTPacket packet;
		int expired = watchdog.expired();

		if (expired != UMT_WD_NO_EXPIRED)
			uinputm.endEvent(expired);

		if (!packetQueue.dequeue(packet))
			continue;

		if (packet.isEndEvent()) {
			ret = watchdog.unwatch(packet.getId());
			if (!ret) {
				UMT_LOG(UMT_ERR_LOG, "Recieved end event for unwatched id!\n");
				continue;
			}
		} else {
			watchdog.watch(packet.getId());
		}

		ret = uinputm.submitEvent(packet);
		if (!ret)
			UMT_LOG(UMT_ERR_LOG, "submitEvent failed, ret %d\n", ret);
	}
}

int main()
{
	UMTQueue<UMTPacket> packetQueue;

	packetQueue.setBlocking(!enable_watchdog);

	UMT_LOG(UMT_INFO_LOG,	"\n"
				"Starting UMT with the following settings: \n"
				"> UART dev: %s\n"
				"> UINPUT dev: %s\n"
				"> Watchdog enabled: %s\n"
				"> Blocking queue: %s\n"
				"> Baud rate: %d\n"
				"> Invalid streaks: %d\n"
				"> Touch timeout: %d ms\n"
				"> Resolution: %dx%d\n"
				"> Finger count: %d\n"
				"\n",
		UMT_UART_DEVICE, UMT_UINPUT_DEVICE, UMT_BOOL_TO_STR(enable_watchdog),
		UMT_BOOL_TO_STR(!enable_watchdog), UMT_BAUD_RATE, UMT_MAX_INVAL_STREAK,
		UMT_WD_TIMEOUT, UMT_UDEV_RES_X_MAX, UMT_UDEV_RES_Y_MAX, UMT_MAX_FINGER_CNT + 1);

	std::thread umtProducer(umt_producer, std::ref(packetQueue));
	std::thread umtConsumer(umt_consumer, std::ref(packetQueue));

	umtProducer.join();
	umtConsumer.join();

	return 0;
}