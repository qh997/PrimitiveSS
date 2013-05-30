#ifndef __STDIO_H__
#define __STDIO_H__

#include "types.h"

#define STR_DEFAULT_LEN 1024

int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);

void disp_str(char *s);
void disp_int(int n);
int early_printk(const char *fmt, ...);

#endif
