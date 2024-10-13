#include "umt.h"
#include "umt_packet.h"

#include <cassert>

static const uint32_t start_value = 0xDEADBEEF;

void UMTPacket::fill()
{
	uart_packet *upack;
	int start_byte_pos = 0;
	int i;

	for (i = 0; i < getSize(); ++i) {
		uint8_t start_byte = (start_value >> (start_byte_pos * 8) & 0xFF);

		UMT_LOG(UMT_DBG_LOG, "start_byte 0x%x, buffer[%d] 0x%x\n", start_byte, i, buffer[i]);

		if (buffer[i] == start_byte)
			start_byte_pos++;

		if (start_byte_pos == sizeof(start_value))
			break;
	}

	if (i == getSize()) {
		UMT_LOG(UMT_DBG_LOG, "Start value not found!\n");
		i = 0;
	} else {
		i += 1;
	}

	assert(i > 0 && i < getSize());
	UMT_LOG(UMT_DBG_LOG, "Packet data starts at index %d\n", i);

	upack = reinterpret_cast<uart_packet *>(&buffer[i]);

	x       = upack->x;
	y       = upack->y;
	id      = upack->id;
	release = upack->release;
	crc     = upack->crc;
}

size_t UMTPacket::getSize() const
{
	return sizeof(buffer);
}

uint8_t *UMTPacket::getBuffer()
{
	return buffer;
}

bool UMTPacket::validate() const
{
	uint16_t crc;
	bool valid ;

	crc   = x + y + id + release;
	valid = (crc == this->crc)                           && 
			(x > 0 && x  <= UMT_UDEV_RES_X_MAX)  &&
			(y > 0 && y <= UMT_UDEV_RES_Y_MAX)   &&
			(id <= UMT_MAX_FINGER_CNT)           &&
			(release == 1 || release == 0);

	UMT_LOG(UMT_DBG_LOG, "Packet CRC %d, calculated CRC %d, sanity checks %s\n",
		crc, this->crc, UMT_BOOL_TO_STR(valid));

	return valid;
}

int UMTPacket::getX()
{
	return x;
}

int UMTPacket::getY()
{
	return y;
}

int UMTPacket::getId()
{
	return id;
}

int UMTPacket::getRelease()
{
	return release;
}

bool UMTPacket::isEndEvent()
{
	return release;
}
