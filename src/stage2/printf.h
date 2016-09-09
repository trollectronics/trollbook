#ifndef __PRINTF_H__
#define	__PRINTF_H__
#include <stdarg.h>

int printf(char *format, ...);
int vprintf(char *format, va_list va);

#endif
