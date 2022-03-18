#ifndef BOOTROM_TIMER
#define BOOTROM_TIMER

struct timer_regs {
	int ip_info[4];
	unsigned int ctrl;
	unsigned int clk_div;
	unsigned int cnt_low;
	unsigned int cnt_high;
	unsigned int cmp_ctrl;
	unsigned int cmp_low;
	unsigned int cmp_high;
	unsigned int cmp_reserved;
	unsigned int irq_en;
	unsigned int irq_pend;
	unsigned int irq_clr;
};

unsigned int timer_arm(unsigned int cmp_high, unsigned int cmp_low);
unsigned int timer_irq_clr(void);
//unsigned int timer_sleep(unsigned int cmp_high, unsigned int cmp_low);

#endif
