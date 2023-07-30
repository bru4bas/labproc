
.section .init
.global start
start:
  /*
   * Verifica priviégio de execução EL2 (HYP) ou EL1 (SVC)
   */
  mrs r0, cpsr
  and r0, r0, #0x1f
  cmp r0, #0x1a
  bne continua

  /*
   * Sai do modo EL2 (HYP)
   */
  mrs r0, cpsr
  bic r0, r0, #0x1f
  orr r0, r0, #0x13
  msr spsr_cxsf, r0
  add lr, pc, #4       // aponta o rótulo 'continua'
  msr ELR_hyp, lr
  eret                 // 'retorna' do privilégio EL2 para o EL1

continua:
  /*
   * Verifica o índice das CPUs
   */
  mrc p15,0,r0,c0,c0,5    // registrador MPIDR
  ands r0, r0, #0xff
  beq core0

// Núcleos #1, #2 e #3 vão executar a partir daqui

trava:
  wfe
  b trava

// Execução do núcleo #0
core0:
  /*
   * Desliga MMU
   */
  mrc p15, 0, r2, c1, c0, 0
  bic r2, #0x1000
  bic r2, #0x0004
  bic r2, #0x0001
  mcr p15, 0, r2, c1, c0, 0

  /*
   * configura os stack pointers
   */
  mov r0, #0x13     // Modo SVC
  msr cpsr_c,r0
  ldr sp, =inicio_stack

  // Continua executando no modo supervisor (SVC), interrupções desabilitadas
  
 /*
  * zera segmento bss
  */
  mov r0, #0
  ldr r1, =inicio_bss
  ldr r2, =fim_bss
loop:
  cmp r1, r2
  bge cont
  str r0, [r1], #4
  b loop
cont:
  /*
   * Executa o sistema
   */
  b main

 .text

/*
 * Desliga gerenciador de memória.
 */
 .global mmu_stop
mmu_stop:
  mrc p15, 0, r2, c1, c0, 0
  bic r2, #0x1000
  bic r2, #0x0004
  bic r2, #0x0001
  mcr p15, 0, r2, c1, c0, 0
  mov pc, lr

/*
 * Limpa a memória cache do TLB
 */
.global tlb_invalida
tlb_invalida:
  mov r2, #0
  mcr p15, 0, r2, c8, c7, 0
  mcr p15, 0, r2, c7, c10, 4
  mov pc, lr

/*
 * Ativa gerenciador de memória
 */
.global mmu_start
mmu_start:
  mvn r2, #0
  bic r2, #0xC
  mcr p15, 0, r2, c3, c0, 0
  mcr p15, 0, r0, c2, c0, 0
  mcr p15, 0, r0, c2, c0, 1
  mrc p15, 0, r2, c1, c0, 0
  orr r2, r2, #0x05
  orr r2, r2, #0x1000
  mcr p15, 0, r2, c1, c0, 0
  mov pc, lr

/*
 * Suspende o processamento por um número de ciclos
 * param r0 Número de ciclos.
 */
.globl delay
delay:
  subs r0, r0, #1
  bne delay
  mov pc, lr

