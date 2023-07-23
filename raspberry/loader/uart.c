
#include <stdint.h>
#include "bcm.h"

void delay(uint32_t dur);

/**
 * Inicia a mini UART para 115200 bps.
 */
void 
uart_init(void) {
   uint32_t sel = GPIO_REG(gpfsel[1]);
   sel = (sel & (~(7<<12))) | (2<<12);
   sel = (sel & (~(7<<15))) | (2<<15);
   GPIO_REG(gpfsel[1]) = sel;

   GPIO_REG(gppud) = 0;
   delay(150);
   GPIO_REG(gppudclk[0]) = (1 << 14) | (1 << 15);
   delay(150);
   GPIO_REG(gppudclk[0]) = 0;

   AUX_REG(enables) = 1;
   MU_REG(cntl) = 0;
   MU_REG(ier) = 0;
   MU_REG(lcr) = 3;           // 8 bits
   MU_REG(mcr) = 0;
   MU_REG(baud) = 270;        // para 115200 bps em 250 MHz
   MU_REG(cntl) = 3;          // habilita TX e RX
}

/**
 * Envia um caractere pela mini UART.
 */
void 
uart_putc(uint8_t c) {
   while((MU_REG(stat) & 0x02) == 0) ;
   MU_REG(io) = c;
}

/**
 * Envia um string pela mini UART.
 */
void 
uart_puts(char *s) {
   while(*s) {
      uart_putc(*s);
      s++;
   }
}

/**
 * Lê um caractere pela mini UART.
 */
uint8_t 
uart_getc(void) {
   while((MU_REG(stat) & 0x01) == 0) ;
   return MU_REG(io);
}

