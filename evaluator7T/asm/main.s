.include "evlt7t.inc"

.section .reset, "ax"
.org 0
/*
 * Vetor de interrupções
 */
   b reset
   b panic                // undefined instruction
   b panic                // swi
   b panic                // abort 1
   b panic                // abort 2
   nop
   b trata_irq            // interruptção
   b panic                // interrupção rápida

panic:
   b panic

reset:
  /*
   * Configura sp e modo do processador
   */
  mov r0, #0b10010      // modo IRQ
  msr cpsr, r0
  mov r13, #0x6000

  mov r0, #0b10011      // modo SRV
  msr cpsr, r0
  mov r13, #0x8000
  b main

.data
/*
 * Contador de ticks do sistema, incrementado a cada 0.1 s.
 */
ticks: .word 0
.global ticks

.text
.global main

/*
 * Contagem para 0.1 segundo em clock de 50 MHz.
 */
.set TIMER_DATA, 4999999

/*
 * Programa principal
 */
main:
   /*
    * Configura I/O 8 (botão) como interrupção.
    */
   ldr r1, =IOPCON
   ldr r0, [r1]
   bic r0, r0, #0b11111
   orr r0, r0, #0b11001    // borda de subida
   str r0, [r1]

   /*
    * Habilita EXTIRQ 0
    */
   ldr r1, =INTMSK
   ldr r0, [r1]
   bic r0, r0, #1             // EXT0
   bic r0, r0, #(1 << 21)     // global
   str r0, [r1]

  /*
   * Configura leds (I/O 4 e 5) como saídas.
   */
  ldr r1, =IOPMOD
  ldr r0, [r1]
  orr r0, r0, #(0b11 << 4)
  str r0, [r1]

  /*
   * Configura Timer 0
   */
  ldr r1, =TDATA0
  ldr r0, =TIMER_DATA
  str r0, [r1]
  ldr r1, =TMOD
  ldr r0, [r1]
  bic r0, r0, #0b111
  orr r0, r0, #0b011
  str r0, [r1]

  /*
   * Habilita Interrupção do Timer 0.
   */
  ldr r1, =INTMSK
  ldr r0, [r1]
  bic r0, r0, #(1 << 10)
  str r0, [r1]

  /*
   * Configura UART 0
   */
  ldr r1, =ULCON0
  ldr r0, =0b111            // 8 bits, sem paridade, clock interno
  str r0, [r1]

  ldr r1, =UCON0
  ldr r0, =0b00001000       // habilita TX
  str r0, [r1]

  ldr r1, =UBRDIV0
  ldr r0, =(26 << 4)        // valor para 57600 em 50 MHz
  str r0, [r1]

  /*
   * Repetir indefinidamente.
   */
loop:
  /*
   * Pisca o primeiro led.
   */
  ldr r1, =IOPDATA
  ldr r0, [r1]
  eor r0, r0, #(1 << 4)
  str r0, [r1]

  /*
   * Espera.
   */ 
  mov r0, #0x100000
espera:
  subs r0, r0, #1
  bne espera

  /*
   * Envia a letra 'b' pela serial
   */
  ldr r1, =UTXBUF0
  mov r0, #'b'
  str r0, [r1]

  b loop


/*
 * Tratamento das interrupções.
 */
trata_irq:
  /*
   * Salva registradores e verifica causa da interrupção.
   */
  push {r0-r2}
  ldr r1, =INTPND
  ldr r0, [r1]                      // r0 contém INTPND
  tst r0, #1 
  beq cont1

  /*
   * Interrupção externa 0
   * Pisca o segundo led.
   */
  ldr r1, =IOPDATA
  ldr r2, [r1]
  eor r2, r2, #(1 << 5)
  str r2, [r1]

cont1:
  tst r0, #(1 << 10)
  beq cont2

  /*
   * Interrupção do timer 0
   * Incrementa ticks
   */
  ldr r1, =ticks
  ldr r2, [r1]
  add r2, r2, #1
  str r2, [r1]

cont2:
  /*
   * Fim do tratamento.
   * Reconhece todas as interrupções.
   */
  ldr r1, =INTPND
  str r0, [r1]
  pop {r0-r2}
  subs pc, lr, #4

