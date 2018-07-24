/*
Copyright (c) 2017 Steven Arnow <s@rdw.se>

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

#include "lz4inflate.h"

/* This thing does no bounds checking. If the file is corrupted, all hell will break loose */
/* It also requires the whole input data set to be present */
void lz4_inflate(const uint8_t *input, uint8_t *output, uint32_t input_len) {
	uint32_t pos, write;
	uint32_t lit, copy, i;
	uint32_t offset;
	
	for (pos = write = 0; pos < input_len - 11;) {
		lit = input[pos] >> 4;
		copy = input[pos] & 0xF, copy += 4;
		pos++;
		if (lit == 15)
			do {
				lit += input[pos];
			} while (input[pos++] == 0xFF);

		for (i = 0; i < lit; i++)
			output[write + i] = input[pos++];
		write += lit;

		if (pos == input_len)
			break;
		offset = input[pos++];
		offset |= (input[pos++] << 8);

		if (copy == 19)
			do {
				copy += input[pos];
			} while (input[pos++] == 0xFF);
		/* WAT? */
		for (i = 0; i < copy; i++)
			output[write + i] = output[write + i - offset];
		write += i;
	}

	while (pos < input_len)
		output[write++] = input[pos++];
}
