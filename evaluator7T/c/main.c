
#include "evlt7t.h"
#include <stdint.h>
#include <stdlib.h>

#include "timer.h"
#include "serial.h"
#include "botao.h"

void delay(uint32_t t);

/**
 * Tratamento das interrupções do ARM.
 */
void __attribute__((interrupt("IRQ")))
trata_irq(void) {
   /*
    * Verifica causa da interrupção.
    */
   uint32_t pend = INTPND;
   if(bit_is_set(pend, 10)) trata_irq_timer0();
   if(bit_is_set(pend, 4)) trata_irq_uart0_tx();
   if(bit_is_set(pend, 0)) trata_irq_ext0();

   /*
    * Reconhece todas as interrupções.
    */
   INTPND = pend;
}

/*
 * Programa principal
 */
int 
main(void) {
   /*
    * Configura led (I/O 4) como saída.
    */
   bit_set(IOPMOD, 4);

   /*
    * Aciona funções de inicialização.
    */
   inicia_botao();
   inicia_timer0();
   inicia_serial();

   puts("Hello people!\r\n");

   /*
    * Repetir indefinidamente.
    */
   for(;;) {
      bit_inv(IOPDATA, 4);
      delay_tempo(5);
   }
}
