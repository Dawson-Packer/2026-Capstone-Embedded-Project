#ifndef UART_LOG_H
#define UART_LOG_H

#include "usart.h"
#include <stdarg.h>

#define BRIGHT_GREEN  "\033[92m"
#define BRIGHT_RED    "\033[91m"
#define BRIGHT_YELLOW "\033[93m"
#define BRIGHT_CYAN   "\033[96m"
#define BRIGHT_WHITE  "\033[97m"
#define RESET         "\033[0m"

#define BOLD "\033[1m"

void Logger_Raw(const char *fmt, ...);
void Logger_Info(const char *fmt, ...);
void Logger_Warn(const char *fmt, ...);
void Logger_Error(const char *fmt, ...);

#endif