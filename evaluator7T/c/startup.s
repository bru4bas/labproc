/*
 * Vetor de interrupções.
 */
.section .reset, "ax"
.org 0
vetor:
b reset         // reset
b panic         // instrução não definida
b panic         // interrupção de software
b panic         // abort de instrução
b panic         // abort de dados
nop
b trata_irq     // interrupção (tratamento em C)
b panic         // interrupção rápida

reset:
  b start

panic:
  b panic

.text
.global delay, start

/*
 * Ponto de entrada do programa.
 */
start:
   /*
    * Configura modo do processador e pilha
    */
   mov r0, #0b10010              // modo IRQ
   msr cpsr, r0
   mov r13, #0x8000

   mov r0, #0b10011              // modo SVR
   msr cpsr, r0
   ldr r13, =inicio_stack

   /*
    * Zera área bss.
    */
   mov r0, #0
   ldr r1, =inicio_bss
   ldr r2, =fim_bss
loop:
   cmp r1, r2
   bge cont
   str r0, [r1], #4
   b loop;
cont:
   b main

/*
 * Função utilitária delay.
 */
delay:
   subs r0, r0, #1
   bne delay
   mov pc, lr

