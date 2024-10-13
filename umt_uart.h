#ifndef __UMT_UART_H__
#define __UMT_UART_H__

#include "umt.h"
#include "umt_fd.h"
#include "umt_packet.h"

#define UMT_BAUD_RATE		115200
#define UMT_MAX_INVAL_STREAK	3

class UARTManager : public FDManager
{
public:
	UARTManager(const char *file, int flags);

	int getPacket(UMTPacket& packet);
	void countInvalidPackets(bool valid);
	void flush();

private:
	int validation_fails;
};

#endif /* __UMT_UART_H__ */
