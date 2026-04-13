#include "logger.h"
#include "usart.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static inline void PrintLog(const char *color, const char *level, const char *fmt, va_list args) {
    printf("%s[%s] ", color, level);
    vprintf(fmt, args);
    printf(RESET "\r\n");
}

void Logger_Raw(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void Logger_Info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    PrintLog(BRIGHT_GREEN, "INFO ", fmt, args);
    va_end(args);
}

void Logger_Warn(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    PrintLog(BRIGHT_YELLOW, "WARN ", fmt, args);
    va_end(args);
}

void Logger_Error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    PrintLog(BRIGHT_RED, "ERROR", fmt, args);
    va_end(args);
}

void _write(int file, char *ptr, int len) {
#ifdef DEBUG
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, 1000);
#endif
}