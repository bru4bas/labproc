
#include <stdint.h>
#include "bcm.h"

char msg[] = "Hello\r\n";

void delay(uint32_t dur);

/**
 * Configura a mini-uart para 115200
 */
void uart_init(void) {
   /*
    * Configura os GPIO 14 e 15 com a função alternativa 2 (UART).
    */
   uint32_t sel = GPIO_REG(gpfsel[1]);
   sel = (sel & (~(7<<12))) | (2<<12);
   sel = (sel & (~(7<<15))) | (2<<15);
   GPIO_REG(gpfsel[1]) = sel;

   /*
    * Configura pullups nos pinos
    */
   GPIO_REG(gppud) = 0;
   delay(150);
   GPIO_REG(gppudclk[0]) = (1 << 14) | (1 << 15);
   delay(150);
   GPIO_REG(gppudclk[0]) = 0;

   /*
    * Configura o periférico da mini uart
    */
   AUX_REG(enables) = 1;
   MU_REG(cntl) = 0;
   MU_REG(ier) = 0;
   MU_REG(lcr) = 3;           // 8 bits
   MU_REG(mcr) = 0;
   MU_REG(baud) = 270;        // para 115200 bps em 250 MHz
   MU_REG(cntl) = 3;          // habilita TX e RX
}

/**
 * Envia um caractere pela uart.
 */
void uart_putc(uint8_t c) {
   while((MU_REG(lsr) & 0x20) == 0) ;
   MU_REG(io) = c;
}

/**
 * Envia um string (terminado em zero) pela uart.
 * Suspende o processamento até terminar.
 */
void uart_puts(char *s) {
   while(*s) {
      uart_putc(*s);
      s++;
   }
}

/**
 * Lê um caractere pela uart.
 * Suspende o processamento até receber.
 */
uint8_t uart_getc(void) {
   while((MU_REG(lsr) & 0x01) == 0) ;
   return MU_REG(io);
}

/**
 * Ponto de entrada do programa.
 * Pisca led e cumprimenta o usuário pela uart.
 */
int main(void) {
   // LED no gpio 47
   uint32_t fsel = GPIO_REG(gpfsel[4]);
   GPIO_REG(gpfsel[4]) = (fsel & (~(0x7 << 21))) | (1 << 21);
   uart_init();

   for(;;) {
      GPIO_REG(gpset[1]) = (1 << (47-32));
      delay(1000000);
      GPIO_REG(gpclr[1]) = (1 << (47-32));
      delay(1000000);
      uart_puts(msg);
   }
   return 0;
}

