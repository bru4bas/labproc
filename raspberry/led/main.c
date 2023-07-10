
#include <stdint.h>
#include "bcm.h"

void delay(uint32_t dur);

/**
 * Pisca o led verde do Raspberry.
 */
int main(void) {
   /*
    * Configura GPIO 47 como saída (bits 21-23 de gpfsel[4] = 001)
    */
   uint32_t fsel = GPIO_REG(gpfsel[4]);
   GPIO_REG(gpfsel[4]) = (fsel & (~(0x7 << 21))) | (1 << 21);

   for(;;) {
      GPIO_REG(gpset[1]) = (1 << (47-32));    // liga saída 47
      delay(1000000);
      GPIO_REG(gpclr[1]) = (1 << (47-32));    // desliga saída 47
      delay(1000000);
   }
   return 0;
}

