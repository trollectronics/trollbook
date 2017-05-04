#ifndef DRAW_UTF8_H_
#define DRAW_UTF8_H_

int draw_utf8_valid(const unsigned char *str);
unsigned int draw_utf8_get_char(const char *str_s);
int draw_utf8_char_length(const char *str_s);
int draw_utf8_find_char_index(const char *str_s, unsigned int pos);
const char *draw_utf8_find_start_by_char_pos(const char *str_s, unsigned int pos);
int draw_utf8_counted_string_size(const char *str_s, unsigned int chars);
int draw_utf8_chars_in_string(const char *str_s);
int draw_utf8_encoded_length(unsigned int ch);
int draw_utf8_encode(unsigned int ch, char *str_s, int buf_len);

#endif
