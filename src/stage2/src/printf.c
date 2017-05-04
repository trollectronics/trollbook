#include <stdarg.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define PRINTF_BOTH

#ifdef PRINTF_BOTH
#include "uart.h"
#include "terminal.h"

static void _printf_putc(char c) {
	uart_putc_convnl(c);
	terminal_putc_term(c);
}

static void _printf_puts(char *s) {
	uart_puts_convnl(s);
	terminal_puts(s);
}

#endif

#ifdef PRINTF_UART
#include "uart.h"
#define _printf_putc(c) uart_putc_convnl((c))
#define _printf_puts(c) uart_puts_convnl((c))
#endif

#ifdef PRINTF_DISPLAY
#include "terminal.h"
#define _printf_putc(c) terminal_putc_term((c))
#define _printf_puts(c) terminal_puts((c))
#endif

static void _printf_put_counted(char *s, int width) {
	while(width--)
		_printf_putc(*s++);
}

static char *int_to_string(unsigned long long int n, char *s, int base) {
	char i;
	do {
		s--;
		i = n  % base;
		i += (i > 9 ? 'A' - 10 : '0');
		*s = i;
		n /= base;
	} while(n);
	return s;
}

int vprintf(char *format, va_list va) {
	unsigned char pad, c;
	int i;
	unsigned int j;
	unsigned long long int num = 0;
	signed long long int signum = 0;
	
	enum {
		LENGTH_CHAR,
		LENGTH_SHORT,
		LENGTH_INT,
		LENGTH_LONG,
		LENGTH_LONG_LONG,
		LENGTH_INTMAX_T,
		LENGTH_SIZE_T,
		LENGTH_PTRDIFF_T,
	} length;
	int width;
	char prefix, base, *s;
	char buf[25];
	buf[24] = 0;
	
	for(i=0; (c = *format++); i++) {
		if(c != '%') {
			_printf_putc(c);
			continue;
		}
		length = LENGTH_INT;
		width = 0;
		prefix = 0;
		pad = ' ';
		base = 10;
		
		while(1) {
			switch(c = *format++) {
				case 0:
					goto end;
				case '%':
					_printf_putc(c);
					goto next;
				case '#':
					prefix = 1;
					break;
				case '0':
					if(!width) {
						pad = '0';
						break;
					}
				case '1': case '2': case '3': case '4':
				case '5': case '6': case '7': case '8': case '9':
					width = width*10 + (c - '0');
					break;
				case 'h':
					length = length == LENGTH_SHORT ? LENGTH_CHAR : LENGTH_SHORT;
					break;
				case 'l':
					length = length == LENGTH_LONG ? LENGTH_LONG_LONG : LENGTH_LONG;
					break;
				case 'j':
					length = LENGTH_INTMAX_T;
					break;
				case 'z':
					length = LENGTH_SIZE_T;
					break;
				case 't':
					length = LENGTH_PTRDIFF_T;
					break;
				case 'o':
					base = 8;
					if(prefix)
						_printf_putc('0');
					goto baseconv;
				case 'p':
					length = sizeof(void *);
					prefix = 1;
				case 'x':
				case 'X':
					base = 16;
					if(prefix)
						_printf_puts("0x");
				case 'u':
					baseconv:
					switch(length) {
						case LENGTH_CHAR:
							num = (unsigned char) va_arg(va, unsigned int);
							break;
						case LENGTH_SHORT:
							num = (unsigned short) va_arg(va, unsigned int);
							break;
						case LENGTH_INT:
							num = (unsigned int) va_arg(va, unsigned int);
							break;
						case LENGTH_LONG:
							num = (unsigned long) va_arg(va, unsigned long);
							break;
						case LENGTH_LONG_LONG:
							num = (unsigned long long) va_arg(va, unsigned long long);
							break;
						default:
							break;
					}
					goto print_num;
				case 'd':
				case 'i':
					switch(length) {
						case LENGTH_CHAR:
							signum = (signed char) va_arg(va, signed int);
							break;
						case LENGTH_SHORT:
							signum = (signed short) va_arg(va, signed int);
							break;
						case LENGTH_INT:
							signum = (signed int) va_arg(va, signed int);
							break;
						case LENGTH_LONG:
							signum = (signed long) va_arg(va, signed long);
							break;
						case LENGTH_LONG_LONG:
							signum = (signed long long) va_arg(va, signed long long);
							break;
						default:
							break;
					}
					if(signum < 0) {
						_printf_putc('-');
						num = -signum;
					} else
						num = signum;
					goto print_num;
				case 's':
					_printf_puts(va_arg(va, char *));
					goto next;
				case 'c':
					_printf_putc((char) va_arg(va, int));
					goto next;
			}
		}
		print_num:
		s = int_to_string(num, buf + 24, base);
		j = 24 + buf - s;
		if(!width)
			width = j;
		else
			while(width > j)  {
				_printf_putc(pad);
				width--;
			}
		_printf_put_counted(s, width);
		next:;
	}
	end:
	return i;
}

int printf(char *format, ...) {
	int ret;
	va_list va;
	va_start (va, format);
	ret = vprintf(format, va);
	va_end(va);
	return ret;
}
