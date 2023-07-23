
#include <stdint.h>
#include <stdbool.h>
#include "bcm.h"
#include "uart.h"

uint8_t buffer[120];
uint8_t size;

void delay(uint32_t dur);
bool processa_hex(uint8_t *buffer, uint8_t size);

#define liga_led()       GPIO_REG(gpset[1]) = (1 << (47-32))
#define desliga_led()    GPIO_REG(gpclr[1]) = (1 << (47-32))

int main(void) {
   uint32_t fsel = GPIO_REG(gpfsel[4]);
   GPIO_REG(gpfsel[4]) = (fsel & (~(0x7 << 21))) | (1 << 21);
   uart_init();
   uart_puts("+----------------------------+\n\r");
   uart_puts("| Kernel Panic Loader (TM)   |\n\r");
   uart_puts("| Send you HEX file and pray |\n\r");
   uart_puts("+----------------------------+\n\r");

   for(;;) {
      /*
       * Aguarda um comando entre ':' e CR/LF.
       * Armazena em buffer[]
       */
      size = 0;
      for(;;) {
         uint8_t b = uart_getc();
         if(b == ':') {
            size = 0;
            liga_led();
            continue;
         }
         if((b == '\r') || (b == '\n')) break;
         if(size < sizeof(buffer)-1) {
            buffer[size] = b;
            size++;
         }
      }

      /*
       * Processa uma linha no formato intel HEX.
       */
      buffer[size] = 0;
      processa_hex(buffer, size);
      desliga_led();
   }
   return 0;
}

