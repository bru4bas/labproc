
#include "bcm.h"

volatile uint32_t ticks;

/**
 * Inicializa o timer para gerar interrupção a cada 1 ms.
 */
void configura_timer(void) {
   TIMER_REG(load) = 1000;             // 1MHz / 1000 = 1kHz
   TIMER_REG(control) = __bit(9)       // habilita free-running counter
                      | __bit(7)       // habilita timer
                      | __bit(5)       // habilita interrupção
                      | __bit(1);      // timer de 23 bits

   IRQ_REG(enable_basic) = __bit(0);   // habilita interrupção básica 0 (timer)
}

/**
 * Configura o gpio 47 para acionar o led verde.
 */
void configura_led(void) {
   /*
    * Configura GPIO 47 como saída (bits 21-23 de gpfsel[4] = 001)
    */
   uint32_t fsel = GPIO_REG(gpfsel[4]);
   GPIO_REG(gpfsel[4]) = (fsel & (~(0x7 << 21))) | (1 << 21);
}

/**
 * Ponto de entrada dos Serviços de Interrupção
 */
void __attribute__((interrupt("IRQ"))) 
trata_irq(void) {
   if(bit_is_set(IRQ_REG(pending_basic), 0)) {
      TIMER_REG(ack) = 1;
      ticks++;
   }
}

/**
 * Monitora o contador de ticks para a passagem de um segundo.
 */
void espera_um_segundo(void) {
   uint32_t inicio = ticks;
   while((ticks - inicio) < 1000) ;
}

/**
 * Ponto de entrada do programa.
 */
int main(void) 
{
   ticks = 0;
   configura_led();
   configura_timer();
   enable_irq(1);
   for(;;) {
      GPIO_REG(gpset[1]) = (1 << (47-32));    // liga saída 47
      espera_um_segundo();
      GPIO_REG(gpclr[1]) = (1 << (47-32));    // desliga saída 47
      espera_um_segundo();
   }
   return 0;
}
