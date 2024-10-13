#include "umt.h"
#include "umt_uinput.h"

#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <linux/uinput.h>

UINPUTManager::UINPUTManager(const char *file, int flags) : FDManager(file, flags)
{
	uinput_user_dev uidev;
	int fd = getFd();

	memset(&uidev, 0, sizeof(uidev));

	strncpy(uidev.name, "UART Multitouch", UINPUT_MAX_NAME_SIZE);
	uidev.id.bustype = BUS_USB;
	uidev.id.vendor  = 0x1;
	uidev.id.product = 0x1;
	uidev.id.version = 0x1;	

	ioctl(fd, UI_SET_EVBIT,  EV_SYN);
	ioctl(fd, UI_SET_EVBIT,  EV_KEY);
	ioctl(fd, UI_SET_KEYBIT, BTN_TOUCH);
	ioctl(fd, UI_SET_EVBIT,  EV_ABS);
	ioctl(fd, UI_SET_ABSBIT, ABS_X);
	ioctl(fd, UI_SET_ABSBIT, ABS_Y);
	ioctl(fd, UI_SET_ABSBIT, ABS_MT_SLOT);
	ioctl(fd, UI_SET_ABSBIT, ABS_MT_POSITION_X);
	ioctl(fd, UI_SET_ABSBIT, ABS_MT_POSITION_Y);
	ioctl(fd, UI_SET_ABSBIT, ABS_MT_TRACKING_ID);

	uidev.absmax[ABS_X]              = UMT_UDEV_RES_X_MAX;
	uidev.absmax[ABS_Y]              = UMT_UDEV_RES_Y_MAX;
	uidev.absmax[ABS_MT_POSITION_X]  = UMT_UDEV_RES_X_MAX;
	uidev.absmax[ABS_MT_POSITION_Y]  = UMT_UDEV_RES_Y_MAX;
	uidev.absmax[ABS_MT_SLOT]        = UMT_MAX_FINGER_CNT;
	uidev.absmax[ABS_MT_TRACKING_ID] = 65535;

	write(fd, &uidev, sizeof(uidev));
	ioctl(fd, UI_DEV_CREATE);

	/* Ensure the device has enough time to be created */
	sleep(1);
}

UINPUTManager::~UINPUTManager()
{
	ioctl(getFd(), UI_DEV_DESTROY);
}

int UINPUTManager::writeEvent(int type, int code, int value)
{
	input_event event;
	timeval time;
	int fd = getFd();

	gettimeofday(&time, NULL);

	event.time.tv_sec  = time.tv_sec;
	event.time.tv_usec = time.tv_usec;
	event.type         = type;
	event.code         = code;
	event.value        = value;

	return write(fd, &event, sizeof(event));
}

void UINPUTManager::generateEvent(int id, int x, int y)
{
	UMT_LOG(UMT_INFO_LOG, "Generate event for { x: %5u, y: %5u, id: %3u }\n", x, y, id);

	writeEvent(EV_ABS, ABS_X, x);
	writeEvent(EV_ABS, ABS_Y, y);

	writeEvent(EV_ABS, ABS_MT_SLOT,        id);
	writeEvent(EV_ABS, ABS_MT_TRACKING_ID, id);
	writeEvent(EV_ABS, ABS_MT_POSITION_X,   x);
	writeEvent(EV_ABS, ABS_MT_POSITION_Y,   y);

	writeEvent(EV_KEY, BTN_TOUCH,  1);
	writeEvent(EV_SYN, SYN_REPORT, 0);
}

void UINPUTManager::endEvent(int id)
{
	UMT_LOG(UMT_INFO_LOG, "End event for id: %d\n", id);

	writeEvent(EV_ABS, ABS_MT_SLOT,        id);
	writeEvent(EV_ABS, ABS_MT_TRACKING_ID, -1);
	writeEvent(EV_SYN, SYN_REPORT,          0);
}

bool UINPUTManager::submitEvent(UMTPacket& packet)
{
	int x  = packet.getX();
	int y  = packet.getY();
	int id = packet.getId();

	generateEvent(id, x, y);

	return true;
}
