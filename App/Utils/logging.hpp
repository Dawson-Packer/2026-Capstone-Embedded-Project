#pragma once

#include <stdio.h>

//=== ANSI Excape Codes =======================
#define ANSI_BRIGHT_CYAN   "\033[96m"
#define ANSI_BRIGHT_GREEN  "\033[92m"
#define ANSI_BRIGHT_RED    "\033[91m"
#define ANSI_BRIGHT_WHITE  "\033[97m"
#define ANSI_BRIGHT_YELLOW "\033[93m"
#define ANSI_BRIGHT_GRAY   "\033[90m"

#define ANSI_BOLD "\033[1m"

#define ANSI_RESET "\033[0m"

#define LOG_DBG(tag, fmt, ...)                                                                     \
    printf(ANSI_BRIGHT_GRAY "TRACE [%s] " fmt ANSI_RESET "\r\n", tag, ##__VA_ARGS__)

#define LOG_INF(tag, fmt, ...)                                                                     \
    printf(ANSI_BRIGHT_GREEN "INFO  [%s] " fmt ANSI_RESET "\r\n", tag, ##__VA_ARGS__)

#define LOG_WRN(tag, fmt, ...)                                                                     \
    printf(ANSI_BRIGHT_YELLOW "WARN  [%s] " fmt ANSI_RESET "\r\n", tag, ##__VA_ARGS__)

#define LOG_ERR(tag, fmt, ...)                                                                     \
    printf(ANSI_BRIGHT_RED "ERROR [%s] " fmt ANSI_RESET "\r\n", tag, ##__VA_ARGS__)