
#ifndef __BCM_H__
#define __BCM_H__

#include <stdint.h>

#define SYSCLOCK_HZ  250000000
#define APBCLOCK_HZ  125000000

#define __bit(X)             (0x01 << X)
#define bit_is_set(X, Y)     (X & (0x01 << Y))
#define bit_not_set(X, Y)    ((X & (0x01 << Y)) == 0)
#define set_bit(X, Y)        X |= __bit(Y)
#define clr_bit(X, Y)        X &= (~__bit(Y))

#define PERIPH_BASE  0x3f000000
//#define PERIPH_BASE  0x20000000 no RPi0 e RPi1
#define GPIO_ADDR    (PERIPH_BASE + 0x200000)
#define AUX_ADDR     (PERIPH_BASE + 0x215000)
#define AUX_MU_ADDR  (PERIPH_BASE + 0x215040)
#define TIMER_ADDR   (PERIPH_BASE + 0x00B400)
#define IRQ_ADDR     (PERIPH_BASE + 0x00B200)

#define CORE_ADDR    0x40000000

typedef struct {
   uint32_t gpfsel[6];   // Function select (3 bits/gpio)
   unsigned : 32;
   uint32_t gpset[2];    // Output set (1 bit/gpio)
   unsigned : 32;
   uint32_t gpclr[2];    // Output clear (1 bit/gpio)
   unsigned : 32;
   uint32_t gplev[2];    // Input read (1 bit/gpio)
   unsigned : 32;
   uint32_t gpeds[2];    // Event detect status
   unsigned : 32;
   uint32_t gpren[2];    // Rising-edge detect enable
   unsigned : 32;
   uint32_t gpfen[2];    // Falling-edge detect enable
   unsigned : 32;
   uint32_t gphen[2];    // High level detect enable
   unsigned : 32;
   uint32_t gplen[2];    // Low level detect enable
   unsigned : 32;
   uint32_t gparen[2];   // Async rising-edge detect
   unsigned : 32;
   uint32_t gpafen[2];   // Async falling-edge detect
   unsigned : 32;
   uint32_t gppud;       // Pull-up/down enable
   uint32_t gppudclk[2]; // Pull-up/down clock enable
} gpio_reg_t;
#define GPIO_REG(X)  ((gpio_reg_t*)(GPIO_ADDR))->X

typedef struct {
   uint32_t irq;
   uint32_t enables;
} aux_reg_t;
#define AUX_REG(X)   ((aux_reg_t*)(AUX_ADDR))->X

typedef struct {
   uint32_t io;
   uint32_t ier;
   uint32_t iir;
   uint32_t lcr;
   uint32_t mcr;
   uint32_t lsr;
   uint32_t msr;
   uint32_t scratch;
   uint32_t cntl;
   uint32_t stat;
   uint32_t baud;
} mu_reg_t;
#define MU_REG(X)    ((mu_reg_t*)(AUX_MU_ADDR))->X

typedef struct {
   uint32_t load;
   uint32_t value;
   uint32_t control;
   uint32_t ack;
   uint32_t raw_irq;
   uint32_t masked_irq;
   uint32_t reload;
   uint32_t pre;
   uint32_t counter;
} timer_reg_t;
#define TIMER_REG(X)   ((timer_reg_t*)(TIMER_ADDR))->X

typedef struct {
   uint32_t pending_basic;
   uint32_t pending_1;
   uint32_t pending_2;
   uint32_t fiq;
   uint32_t enable_1;
   uint32_t enable_2;
   uint32_t enable_basic;
   uint32_t disable_1;
   uint32_t disable_2;
   uint32_t disable_basic;
} irq_reg_t;
#define IRQ_REG(X)     ((irq_reg_t*)(IRQ_ADDR))->X

typedef struct {
   uint32_t control;
   unsigned : 32; 
   uint32_t timer_pre;
   uint32_t irq_routing;
   uint32_t pmu_irq_enable;
   uint32_t pmu_irq_disable;
   unsigned : 32;
   uint32_t timer_count_lo;
   uint32_t timer_count_hi;
   uint32_t local_routing;
   unsigned : 32;
   uint32_t axi_counters;
   uint32_t axi_irq;
   uint32_t timer_control;
   uint32_t timer_flags;
   unsigned : 32;
   uint32_t timer_irq[4];
   uint32_t mailbox_irq[4];
   uint32_t irq_source[4];
   uint32_t fiq_source[4];
   uint32_t core0_mailbox_write[4];
   uint32_t core1_mailbox_write[4];
   uint32_t core2_mailbox_write[4];
   uint32_t core3_mailbox_write[4];
   uint32_t core0_mailbox_read[4];
   uint32_t core1_mailbox_read[4];
   uint32_t core2_mailbox_read[4];
   uint32_t core3_mailbox_read[4];
} core_reg_t;
#define CORE_REG(X)     ((core_reg_t*)(CORE_ADDR))->X

void delay(uint32_t dur);
uint32_t get_cpsr(void);
void enable_irq(uint32_t en);

#endif
