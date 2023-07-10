
#include "evlt7t.h"
#include <stdint.h>
#include <stdlib.h>

/**
 * Base de tempo do sistema, incrementado a cada 0.1 segundos.
 */
volatile static uint32_t ticks;

/**
 * Tratamento da interrupção do timer 0.
 * Chamado por trata_irq().
 */
void 
trata_irq_timer0(void) {
   ticks++;
}

/**
 * Lê o valor atual dos ticks com segurança.
 * @return Valor atual do contador de ticks.
 */
uint32_t 
get_ticks(void) {
   uint32_t res;
   bit_set(INTMSK, 10);
   res = ticks;
   bit_clr(INTMSK, 10);
   return res;
}

/**
 * Espera ocioso com base no contador ticks.
 * @param v Tempo a esperar em unidades de 0.1s.
 */
void 
delay_tempo(uint32_t v) {
   uint32_t inicio = get_ticks();
   while((get_ticks() - inicio) < v) ;
}

/**
 * Configura o timer0 para gerar interrupção a cada 100ms.
 */
void
inicia_timer0(void) {
   TDATA0 = 4999999;        // valor para 0.1s com clock de 50 MHz
   TCNT0 = TDATA0;
   TMOD = (TMOD & (~0b111)) | 0b011;  // ativa o timer 0
   bit_clr(INTMSK, 10);     // habilita interrupção do timer 0
   bit_clr(INTMSK, 21);     // habilita interrupções globais 
}
