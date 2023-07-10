
/**
 * Vetor de interrupções do ARM
 */
.section .reset, "ax"
.org 0
vetor:
   ldr pc, reset_addr         // reset
   b panic                    // instrução não definida
   ldr pc, swi_addr           // interrupção de software (chamada do kernel)
   b panic                    // abort de instrução
   b panic                    // abort de dados
   nop
   b panic                    // interrupção
   b panic                    // interrupção rápida

panic:
   b panic

reset_addr: .word reset
swi_addr:   .word trata_swi

.text
/*
 * Ponto de entrada após reset.
 */
reset:
   // Configura sp do modo SVR:
   mov r0, #0b10011
   msr cpsr, r0
   ldr sp, =stack_svr

   // Zera segmento .bss:
   mov r0, #0
   ldr r1, =inicio_bss
   ldr r2, =fim_bss
loop_zera:
   cmp r1, r2
   bge start
   str r0, [r1], #4
   b loop_zera

/*
 * Ponto de entrada: executa o primeiro thread (tid = 0).
 */
start:
   mov r0, #0              // tid = 0
   ldr r1, =tid
   str r0, [r1]
   ldr r0, =tcb            // curr_tcb = &tcb[0]
   ldr r1, =curr_tcb
   str r0, [r1]
   b context_change

/*
 * Ponto de entrada do kernel.
 * Identifica a função solicitada e trata.
 */
trata_swi:
   cmp r0, #1          // função yield: troca de thread
   beq thread_switch

   cmp r0, #2          // função getpid: retorna a identificação do thread atual
   beq getid

   // outras funções do kernel vão aqui...
   movs pc, lr          // retorna

/*
 * Retorna a identificação do thread atual.
 */
getid:
   ldr r0, =tid
   ldr r0, [r0]
   movs pc, lr

/*
 * Troca de contextos.
 * Aqui é onde a mágica acontece
 */
thread_switch:
   /*
    * Salva o contexto do usuário no tcb
    */
   push {r0}
   ldr r0, =curr_tcb
   ldr r0, [r0]
   stmib r0, {r1-r14}^          // registradores r1-r14 do usuário
   
   // salva endereço de retorno (lr)
   str lr, [r0, #60]

   // copia o spsr do usuário em r1 e salva no tcb
   mrs r1, spsr
   str r1, [r0, #64]
   
   // finalmente, salva o r0 original no tcb
   pop {r1}
   str r1, [r0]

   // escala o próximo processo 
   bl schedule

context_change:
   /*
    * Retorna no conexto de outro thread
    */
   ldr r0, =curr_tcb
   ldr r0, [r0]

   // restaura spsr do usuário.
   ldr r1, [r0, #64]
   msr spsr, r1

   // Restaura r1-r14 originais, lr de retorno e finalmente r0
   ldmib r0, {r1-r14}^
   ldr lr, [r0, #60]
   ldr r0, [r0]

   // retorna para o thread, mudando o modo 
   movs pc, lr
