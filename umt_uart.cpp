#include "umt_uart.h"

#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <linux/serial.h>
#include <sys/ioctl.h>

UARTManager::UARTManager(const char *file, int flags) : FDManager(file, flags)
{
	int fd = getFd();
	serial_struct serial;

	ioctl(fd, TIOCGSERIAL, &serial);
	serial.baud_base  = UMT_BAUD_RATE;
	serial.flags     |= ASYNC_LOW_LATENCY;
	ioctl(fd, TIOCSSERIAL, &serial);

	flush();
}

int UARTManager::getPacket(UMTPacket& packet)
{
	uint8_t *buff = packet.getBuffer();
	size_t size   = packet.getSize();
	int fd        = getFd();
	bool valid;

	if (read(fd, buff, size) < 1) {
		UMT_LOG(UMT_DBG_LOG, "reading failed\n");
		return -EAGAIN;
	}

	packet.fill();
	valid = packet.validate();
	countInvalidPackets(valid);
	if (!valid) {
		UMT_LOG(UMT_DBG_LOG, "Invalid packet! Skipping...\n");

		UMT_LOG(UMT_DBG_LOG, "\n=== Dumping buffer ===\n");
		UMT_BUFF_DUMP(reinterpret_cast<char *>(buff), size);
		UMT_LOG(UMT_DBG_LOG, "=== Dumping END ===\n\n");

		return -EINVAL;
	}

	return 0;
}

void UARTManager::countInvalidPackets(bool valid)
{
	if (validation_fails < UMT_MAX_INVAL_STREAK) {
		validation_fails += valid ? -1 : 1;
		if (validation_fails < 0)
			validation_fails = 0;

		UMT_LOG(UMT_DBG_LOG, "validation_fails %d\n", validation_fails);
	} else {
		validation_fails = 0;
		UMT_LOG(UMT_ERR_LOG, "A streak of invalid packets detected! Flushing...\n");
		flush();
	}
}

void UARTManager::flush()
{
	UMT_LOG(UMT_DBG_LOG, "Flushing...\n");
	tcflush(getFd(), TCIOFLUSH);
}
