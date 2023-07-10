
#pragma once

#include <stdint.h>

void trata_irq_timer0(void);
uint32_t get_ticks(void);
void delay_tempo(uint32_t v);
void inicia_timer0(void);
