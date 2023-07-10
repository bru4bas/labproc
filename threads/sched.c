
#include <stdint.h>

// Definidos pelo linker:
extern uint8_t stack_usr1[];
extern uint8_t stack_usr2[];

// Pontos de entrada dos threads
int main(void);
int main2(void);

/**
 * Estrutura do
 * Thread control block (TCB).
 */
typedef struct {
   uint32_t regs[17];            // contexto
} tcb_t;

/**
 * Threads definidos no sistema.
 */
tcb_t tcb[2] = { 
   /*
    * Contexto do primeiro thread (main).
    */
   {   
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     // r0-r12
      (uint32_t)stack_usr1,                      // sp
      0,                                         // lr inicial
      (uint32_t)main,                            // pc = lr = ponto de entrada
      0x10                                       // valor do cpsr (modo usuário)
   },
   /*
    * Contexto do segundo thread (main2).
    */
   {   
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     // r0-r12
      (uint32_t)stack_usr2,                      // sp
      0,                                         // lr inicial
      (uint32_t)main2,                           // pc = lr = ponto de entrada
      0x10                                       // valor do cpsr (modo usuário)
   }
};

/**
 * Chama o kernel com swi, a função "yield" (r0 = 1).
 * Devolve o controle ao sistema executivo, que pode escalar outro thread.
 */
void __attribute__((naked)) yield(void) {
   asm volatile("push {lr}  \n\t"
                "mov r0, #1 \n\t"
                "swi #0     \n\t"
                "pop {pc}");
}

/**
 * Retorna o thread-id do thread atual.
 */
int __attribute__((naked)) getpid(void) {
   asm volatile("push {lr}  \n\t"
                "mov r0, #2 \n\t"
                "swi #0     \n\t"
                "pop {pc}");
}

volatile int tid;                   // identificador do thread atual (0 ou 1)
volatile void *curr_tcb = &tcb[0];  // tcb do thread atual

/**
 * Escalador:
 * Escolhe o próximo thread.
 */
void schedule(void) {
   if(tid == 0) {
      tid = 1;
      curr_tcb = &tcb[1];
   } else {
      tid = 0;
      curr_tcb = &tcb[0];
   }
}

