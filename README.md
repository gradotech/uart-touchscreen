# UART Touch Screen (UART Multitouch UMT)

This project contains an Android userspace driver that reads coordinates from UART and uses them to generate touchscreen inputs.

The packets recieved should follow the following format:
```c
struct __attribute__((__packed__)) uart_packet {
	uint16_t x;
	uint16_t y;
	uint8_t id;
	uint8_t release;
	uint16_t crc;
};
```

Where:
- **x** and **x** - the coordinates;
- **id** - the id of the finger the event belongs to;
- **release** - can be either 1 (touching) or 0 (not touching);
- **crc** - sum of the above.

The packets should be preceeded by a 32-bit start value (default is 0xDEADBEEF).

Developed by [Grado Technologies](https://gradotech.eu/) (customers@gradotech.eu)
