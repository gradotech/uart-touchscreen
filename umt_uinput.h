#ifndef __UMT_UINPUT_H__
#define __UMT_UINPUT_H__

#include "umt.h"
#include "umt_fd.h"
#include "umt_packet.h"

class UINPUTManager : public FDManager
{
public:
	UINPUTManager(const char *file, int flags);
	~UINPUTManager();

	bool submitEvent(UMTPacket& packet);
	void endEvent(int id);

private:
	int writeEvent(int type, int code, int value);
	void generateEvent(int id, int x, int y);
};

#endif /* __UMT_UINPUT_H__ */
