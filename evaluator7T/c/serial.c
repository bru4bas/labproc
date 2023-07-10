
#include "evlt7t.h"
#include <stdint.h>
#include <stdlib.h>

/*
 * buffer de transmissão
 */
static volatile uint32_t ntx = 0;        ///< Quantidade de bytes a transmitir
static volatile uint8_t *ptx = NULL;     ///< Ponteiro para o próximo byte a transmitir

/**
 * Tratamento da interrupção de transmissão da UART 0.
 * Função chamada por trata_irq().
 */
void 
trata_irq_uart0_tx(void) {
   if(ntx == 0) return;
   ntx--;
   if(ntx == 0) return;
   ptx++;
   UTXBUF0 = *ptx;
}

/**
 * libc não está presente.
 */
int 
strlen(char *s) {
   int n = 0;
   while(*s) {
      n++;
      s++;
   }
   return n;
}

/**
 * Envia um string através da serial, usando interrupções.
 * @param str String a enviar.
 */
void 
puts(char *str) {
   while(ntx) ;             // verifica transmissão pendente
   ntx = strlen(str);
   ptx = str;
   UTXBUF0 = str[0];        // envia o primeiro caractere.
}

/**
 * Configura a UART 0 (serial do usuário).
 * (somente transmissão)
 */
void 
inicia_serial(void) {
   ULCON0 = 0b111;           // 8 bits, sem paridade, clock interno
   UCON0 = 0b1000;           // TX habilitado
   UBRDIV0 = (26 << 4);      // valor para 57600 em 50 MHz
   bit_clr(INTMSK, 4);       // habilita interrupção da transmissão.
}
