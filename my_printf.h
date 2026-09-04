#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdio.h>

#define MSGERR(msg) B_RED(msg)
#define MSGOK(msg) B_GREEN(msg)

void my_printf(const char *format_msg, ...);
void my_printfp(const char *prefix, const char *format_msg, va_list args);

#ifdef __cplusplus
}
#endif