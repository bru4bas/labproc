
#pragma once

#include <stdint.h>

/*
 * Endereços dos registradores GPIO.
 */
#define IOPMOD  (*(uint32_t*)0x03ff5000)
#define IOPCON  (*(uint32_t*)0x03ff5004)
#define IOPDATA (*(uint32_t*)0x03ff5008)

/*
 * Endereços dos registradores do controlador de interrupções.
 */
#define INTMOD (*(uint32_t*)0x03ff4000)
#define INTPND (*(uint32_t*)0x03ff4004)
#define INTMSK (*(uint32_t*)0x03ff4008)

/*
 * Endereços dos registradores dos Timers.
 */
#define TMOD   (*(uint32_t*)0x03ff6000)
#define TDATA0 (*(uint32_t*)0x03ff6004)
#define TDATA1 (*(uint32_t*)0x03ff6008)
#define TCNT0  (*(uint32_t*)0x03ff600c)
#define TCNT1  (*(uint32_t*)0x03ff6010)

/*
 * Contagem para 0.5 segundo em clock de 50 MHz.
 */
#define TIMER_DATA  25000000

/*
 * Endereços dos registradores da UART 0.
 */
#define ULCON0  (*(uint32_t*)0x03ffd000)
#define UCON0   (*(uint32_t*)0x03ffd004)
#define USTAT0  (*(uint32_t*)0x03ffd008)
#define UTXBUF0 (*(uint32_t*)0x03ffd00c)
#define URXBUF0 (*(uint32_t*)0x03ffd010)
#define UBRDIV0 (*(uint32_t*)0x03ffd014)

/*
 * Macros úteis
 */
#define bit_set(X, B) X |= (1 << B)
#define bit_clr(X, B) X &= ~(1 << B)
#define bit_inv(X, B) X ^= (1 << B)
#define bit_is_set(X, B) ( X & (1 << B) )

