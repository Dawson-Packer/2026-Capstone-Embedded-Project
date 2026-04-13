#include "usart.h"
#include <stdarg.h>

void _write(int file, char *ptr, int len) {
#ifdef DEBUG
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, 1000);
#endif
}