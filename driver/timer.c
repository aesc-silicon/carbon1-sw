#include "timer.h"
#include "soc.h"

unsigned int timer_sleep(unsigned int cmp_high, unsigned int cmp_low)
{
	volatile struct timer_regs *timer = (struct timer_regs *)TIMER_BASE;

	timer->cmp_low = cmp_low;
	timer->cmp_high = cmp_high;
	timer->cmp_ctrl = 0x1;

	while (!(timer->irq_pend & 0x1));
	timer->irq_clr = 0x1;

	return 1;
}
