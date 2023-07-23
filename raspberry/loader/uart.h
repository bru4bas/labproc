
#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>

void uart_init(void);
void uart_putc(uint8_t c);
void uart_puts(char *s);
uint8_t uart_getc(void);

#endif
