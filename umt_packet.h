#ifndef __UMT_PACKET_H__
#define __UMT_PACKET_H__

#include <stdint.h>

struct __attribute__((__packed__)) uart_packet {
	uint16_t x;
	uint16_t y;
	uint8_t id;
	uint8_t release;
	uint16_t crc;
};

class UMTPacket
{
public:
	void fill();
	size_t getSize() const;
	uint8_t *getBuffer();
	bool validate() const;

	int getX();
	int getY();
	int getId();
	int getRelease();
	bool isEndEvent();

private:
	int x;
	int y;
	int id;
	int release;
	uint16_t crc;

	uint8_t buffer[sizeof(uart_packet) * 2];
};

#endif /* __UMT_PACKET_H__ */