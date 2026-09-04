#include "my_printf.h"

#include <stdio.h>
#include <cstring>
#include "main.h"
#if defined(AZURE_RTOS_THREADX)
#include "app_threadx.h"
#include "tx_api.h"
#endif
#include "cmsis_gcc.h" // this has to be included last - not sure why but it has to

#if defined(AZURE_RTOS_THREADX)
enum ExecutionContext : uint8_t {
	CONTEXT_ISR,
	CONTEXT_THREADX,
	CONTEXT_MAIN
};

extern TX_MUTEX my_printfMutex;
static ExecutionContext get_execution_context(void);

static ExecutionContext get_execution_context(void) {
	if (__get_IPSR() != 0) return CONTEXT_ISR;
	if (tx_thread_identify() != TX_NULL) return CONTEXT_THREADX;
	return CONTEXT_MAIN;
}

#endif

void my_printf(const char *format_msg, ...) {
	#if defined(AZURE_RTOS_THREADX)
	ExecutionContext executionContext = get_execution_context();
	if (executionContext == CONTEXT_ISR) _tx_thread_context_save();
	UINT txStatus;
	UNUSED(txStatus);
	if (executionContext != CONTEXT_MAIN) {
		txStatus = tx_mutex_get(&my_printfMutex, TX_WAIT_FOREVER);
	}
	#endif

	va_list args;
	va_start(args, format_msg);
	vprintf(format_msg, args);
	va_end(args);

	#if defined(AZURE_RTOS_THREADX)
	if (executionContext != CONTEXT_MAIN) {
		txStatus = tx_mutex_put(&my_printfMutex);
	}
	if (executionContext == CONTEXT_ISR) _tx_thread_context_restore();
	#endif
}

void my_printfp(const char *prefix, const char *format_msg, va_list args) {
	#if defined(AZURE_RTOS_THREADX)
	ExecutionContext executionContext = get_execution_context();
	if (executionContext == CONTEXT_ISR) _tx_thread_context_save();
	UINT txStatus;
	UNUSED(txStatus);
	if (executionContext != CONTEXT_MAIN) {
		txStatus = tx_mutex_get(&my_printfMutex, TX_WAIT_FOREVER);
	}
	#endif

	if (prefix != NULL && 0 != strcmp(format_msg, "\n")) printf(prefix);
	vprintf(format_msg, args);

	#if defined(AZURE_RTOS_THREADX)
	if (executionContext != CONTEXT_MAIN) {
		txStatus = tx_mutex_put(&my_printfMutex);
	}
	if (executionContext == CONTEXT_ISR) _tx_thread_context_restore();
	#endif
}