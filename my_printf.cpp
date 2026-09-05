/*---------------------------------------------------------------------------------------------------
 *  Copyright (c) 2023-2026 Tomasz Jastrzębski. All rights reserved.
 *-------------------------------------------------------------------------------------------------*/

#include "my_printf.h"

#include <stdio.h>
#include <cstring>
#include "main.h"
#if defined(AZURE_RTOS_THREADX)
#include "app_threadx.h"
#include "tx_api.h"
#endif

void my_printf(const char *format_msg, ...) {
#if defined(AZURE_RTOS_THREADX)
	UINT old_posture = tx_interrupt_control(TX_INT_DISABLE);
#endif
	va_list args;
	va_start(args, format_msg);
	vprintf(format_msg, args);
	va_end(args);
#if defined(AZURE_RTOS_THREADX)
	tx_interrupt_control(old_posture);
#endif
}

void my_printfp(const char *prefix, const char *format_msg, va_list args) {
#if defined(AZURE_RTOS_THREADX)
	UINT old_posture = tx_interrupt_control(TX_INT_DISABLE);
#endif
	if (prefix != NULL && 0 != strcmp(format_msg, "\n")) printf(prefix);
	vprintf(format_msg, args);
#if defined(AZURE_RTOS_THREADX)
	tx_interrupt_control(old_posture);
#endif
}
