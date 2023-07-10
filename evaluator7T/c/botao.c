
#include "evlt7t.h"
#include <stdint.h>
#include <stdlib.h>

/**
 * Tratamento da interrupção externa 0 (sinal P8, conectado ao botão).
 * Pisca o segundo led.
 */
void
trata_irq_ext0(void) {
   bit_inv(IOPDATA, 5);
}

/**
 * Função de inicialização do tratamento da interrupção do botão.
 */
void 
inicia_botao(void) {
   /*
    * Configura a saída do led.
    */
   bit_set(IOPMOD, 5);     // configura o pino 5 como saída (segundo led).
   bit_clr(IOPDATA, 5);    // inicia o led apagado.

   /*
    * Configura P8 como entrada com interrupção.
    */
   bit_clr(IOPMOD, 8);
   IOPCON = (IOPCON & ~(0b11111)) | 0b11001;   // borda de subida
   bit_clr(INTMSK, 0);      // habilita interrupção externa 0
}
