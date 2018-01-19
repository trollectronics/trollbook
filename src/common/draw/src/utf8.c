/*
Copyright (c) 2011-2013 Steven Arnow
Adapted from libdarnit for draw in 2017 by Axel Isaksson

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

#include <stddef.h>
#include <draw/utf8.h>

#define UTF8_REPLACEMENT_CHAR 0xFFFD
#define UTF8_CHAR_LIMIT 0x10FFFF

static int get_char_len(const unsigned char *str) {
	if (!((*str & 0xC0) ^ 0x80))
		return 0;
	if (!(*str & 0x80))
		return 1;
	if (!((*str & 0xE0) ^ 0xC0))
		return 2;
	if (!((*str & 0xF0) ^ 0xE0))
		return 3;
	if (!((*str & 0xF8) ^ 0xF0))
		return 4;
	
	return 0;
}


int draw_utf8_valid(const unsigned char *str) {
	int i, j;

	j = get_char_len(str);
	if (j == 0)
		return -1;
	
	for (i = 1; i < j; i++)
		if (((str[i] & 0xC0) ^ 0x80))
			return -1;
	if (str[0] == 0xC0 || str[i] == 0xC1)
		return -1;
	if (str[0] >= 0xF5)
		return -1;
	
	return 0;
}


/* Okay to call */
unsigned int draw_utf8_get_char(const char *str_s) {
	const unsigned char *str = (const unsigned char *) str_s;
	int i;
	unsigned int chr, len, shift;

	if ((len = get_char_len(str)) == 0)
		return UTF8_REPLACEMENT_CHAR;
	if (draw_utf8_valid(str) == -1)
		return UTF8_REPLACEMENT_CHAR;

	shift = 1;
	if (len > 1) shift += len;
		chr = (*str & (0xFF >> shift));
	chr <<= (len-1) * 6;

	for (i = 1; i < len; i++) 
		chr += ((unsigned int) str[i] & 0x3F) << (len - i - 1) * 6;

	return (chr > UTF8_CHAR_LIMIT) ? UTF8_REPLACEMENT_CHAR : chr;
}


/* Okay to call */
int draw_utf8_char_length(const char *str_s) {
	int len;

	if (*str_s == 0)
		return 0;
	
	len = get_char_len((const unsigned char *) str_s);
	if (draw_utf8_valid((const unsigned char *) str_s) == -1)
		return 1;
	
	return (len > 0) ? len : 1;
}


/* Okay to call */
int draw_utf8_find_char_index(const char *str_s, unsigned int pos) {
	int i, j;

	for (i = j = 0; str_s[i] != 0 && j != pos; j++)
		i += draw_utf8_char_length(&str_s[i]);
	return (j != pos) ? -1 : i;
}


/* Okay to call */
const char *draw_utf8_find_start_by_char_pos(const char *str_s, unsigned int pos) {
	int i;

	i = draw_utf8_find_char_index(str_s, pos);
	return (i != -1) ? &str_s[i] : NULL;
}


/* Okay to call */
int draw_utf8_counted_string_size(const char *str_s, unsigned int chars) {
	int i, j;

	for (i = j = 0; i < chars; i++)
		j += draw_utf8_char_length(&str_s[j]);
	return j;
}


/* Okay to call */
int draw_utf8_chars_in_string(const char *str_s) {
	int i, j;

	for (i = j = 0; str_s[i] != 0; j++)
		i += draw_utf8_char_length(&str_s[i]);
	return j;
}


int draw_utf8_encoded_length(unsigned int ch) {
	if (ch < 0x80)
		return 1;
	if (ch < 0x800)
		return 2;
	if (ch < 0x10000)
		return 3;
	return 4;
}


int draw_utf8_encode(unsigned int ch, char *str_s, int buf_len) {
	int i, j;
	unsigned char *str = (unsigned char *) str_s;

	if ((i = j = draw_utf8_encoded_length(ch)) == 1) {
		*str = ch;
		return 1;
	}

	if (buf_len < i)
		return 0;

	for (; j > 1; j--)
		str[j-1] = 0x80 | (0x3F & (ch >> ((i - j) * 6)));

	*str = (~0) << (8 - i);
	*str |= (ch >> (i * 6 - 6));


	return i;
}
