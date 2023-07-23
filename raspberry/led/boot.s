
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
   * configura os stack pointers
   */
  mov r0, #0xD2     // Modo IRQ
  msr cpsr_c,r0
  mov sp, #0x4000

  mov r0, #0xD3     // Modo SVC
  msr cpsr_c,r0
  mov sp, #0x8000

  // Continua executando no modo supervisor (SVC), interrupções desabilitadas

  /*
   * Executa o sistema
   */
  b main

/*
 * Suspende o processamento por um número de ciclos
 * param r0 Número de ciclos.
 */
 .text
.globl delay
delay:
  subs r0, r0, #1
  bne delay
  mov pc, lr

/*
 * Habilita ou desabilita interrupções
 * param r0 0 = desabilita, diferente de zero = habilita
 */
.globl enable_irq
enable_irq:
  movs r0, r0
  beq disable
  mrs r0, cpsr
  bic r0, r0, #0x80
  msr cpsr_c, r0
  mov pc, lr
disable:
  mrs r0, cpsr
  orr r0, r0, #0x80
  msr cpsr_c, r0
  mov pc, lr

/*
 * Lê o valor atual do CPSR
 */
.globl get_cpsr
get_cpsr:
  mrs r0, cpsr
  mov pc, lr

