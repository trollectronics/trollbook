/*
Copyright (c) 2016 Steven Arnow <s@rdw.se>
Modified 2016 by Axel Isaksson for Trollbook bios
'hexload.c' - This file is part of ihextools

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* XXX: This loader will only work on 32 bit systems! */

enum HexloadState {
	HEXLOAD_STATE_INIT,
	HEXLOAD_STATE_BYTE,
	HEXLOAD_STATE_ADDRESS,
	HEXLOAD_STATE_TYPE,
	HEXLOAD_STATE_DATA,
	HEXLOAD_STATE_CHECKSUM,
	HEXLOAD_STATE_SKIP,
};

int hexload_verify_byte(volatile uint8_t *addr, uint8_t data, void *arg) {
	if(*addr == data)
		return 0;
	return -1;
}

int hexload_write_byte(volatile uint8_t *addr, uint8_t data, void *arg) {
	*addr = data;
	return 0;
}

int hexload(uint8_t (* fetch_byte)(int), int fd, int (data_byte)(volatile uint8_t *addr, uint8_t data, void *arg), void *data_byte_arg, void **addr_out) {
	enum HexloadState state = HEXLOAD_STATE_INIT;
	void *entry_point = (void *) 0x10000;
	volatile uint8_t *next = (void *) 0x0;
	uint8_t byte;
	int count;
	uint8_t decoded;
	int bytes_to_read;
	uint16_t addr = 0;
	uint8_t type = 0;
	bool quit = 0;
	
	fetch_byte(-1);
	
	for (count = 0;; count++) {
		switch (state) {
			case HEXLOAD_STATE_INIT:
				if ((byte = fetch_byte(fd)) != ':') {
					if (byte == '\n')
						continue;
					state = HEXLOAD_STATE_SKIP;
					continue;
				} else {
					state = HEXLOAD_STATE_BYTE;
				}
				
				break;
			case HEXLOAD_STATE_BYTE:
				bytes_to_read = decoded;
				count = 0;
				state = HEXLOAD_STATE_ADDRESS;
				break;
			case HEXLOAD_STATE_ADDRESS:
				if (count == 1)
					addr = decoded << 8;
				else {
					addr |= decoded;
					state = HEXLOAD_STATE_TYPE;
				}
				
				break;
			case HEXLOAD_STATE_TYPE:
				type = decoded;
				state = HEXLOAD_STATE_DATA;
				count = 0;
				break;
			case HEXLOAD_STATE_DATA:
				if (type == 0) {
					if(data_byte(next + addr, decoded, data_byte_arg) < 0) {
						if(addr_out)
							*addr_out = (void *) (next + addr);
						return -1;
					}
					addr++;
				} else if (type == 1) {
					state = HEXLOAD_STATE_SKIP;
					quit = true;
					continue;
				} else if (type == 4) {
					if (count == 1)
						next = 0, next += (decoded << 24);
					else
						next += (decoded << 16);
				} else if (type == 5) {
					if (count == 1)
						entry_point = 0;
					entry_point += (decoded << ((4 - count) << 3));
				} else {
					if(addr_out)
						*addr_out = NULL;
					return -1;
				}
				if (count == bytes_to_read)
					state = HEXLOAD_STATE_CHECKSUM;
				break;
			case HEXLOAD_STATE_CHECKSUM:
				//TODO: Check checksum
				state = HEXLOAD_STATE_SKIP;
				continue;
				break;
			case HEXLOAD_STATE_SKIP:
				if (((byte = fetch_byte(fd))) == '\n') {
					if (quit)
						goto load_done;
					state = HEXLOAD_STATE_INIT;
					continue;
				}
				continue;
				break;
		}
		
		/* Decode a byte */
		byte = fetch_byte(fd);
		if (byte > '9')
			decoded = (byte - 0x37);
		else
			decoded = byte - 0x30;
		decoded <<= 4;
		byte = fetch_byte(fd);
		if (byte > '9')
			decoded |= (byte - 0x37);
		else
			decoded |= byte - 0x30;
	}
	
	load_done:
	if(addr_out)
		*addr_out = entry_point;
	
	return 0;
}
