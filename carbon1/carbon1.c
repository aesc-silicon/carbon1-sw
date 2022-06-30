#include "soc.h"
#include "uart.h"
#include "gpio.h"
#include "i2c.h"
#include "mtimer.h"
#include "plic.h"

extern void hang(void);
extern void timer_enable(void);
extern void timer_disable(void);
extern void interrupt_enable(void);
extern void interrupt_disable(void);

/*

  IRS handler is written in assembler due low stack size.

static struct uart_driver uartStd;

void isr_handle(unsigned int mcause)
{
	unsigned char chr;

	interrupt_disable();
	if (uart_irq_rx_ready(&uartStd)) {
		uart_irq_rx_disable(&uartStd);

		while (uart_getc(&uartStd, &chr) == 0) {
			uart_putc(&uartStd, chr);
		}

		uart_irq_rx_enable(&uartStd);
	}

	interrupt_enable();
}
*/

void _kernel(void)
{
	struct uart_driver uartStd;
	struct gpio_driver gpioStatus;
	struct mtimer_driver mtimer;
	struct i2c_driver i2cA;
	struct plic_driver plic;

	plic_init(&plic, PLIC_BASE);
	uart_init(&uartStd, UARTSTD_BASE,
		  UART_CALC_FREQUENCY(UARTSTD_FREQ, UARTSTD_BAUD, 8));
	mtimer_init(&mtimer, MTIMER_BASE);
	gpio_init(&gpioStatus, GPIOSTATUS_BASE);
	i2c_init(&i2cA, I2CA_BASE,
		 I2C_CALC_CLOCK(I2CA_FREQ, I2C_SPEED_STANDARD));

	interrupt_enable();
	plic_irq_enable(&plic, 2);

	uart_putc(&uartStd, 'V');
	uart_putc(&uartStd, 'e');
	uart_putc(&uartStd, 'x');
	uart_putc(&uartStd, 'R');
	uart_putc(&uartStd, 'i');
	uart_putc(&uartStd, 's');
	uart_putc(&uartStd, 'c');
	uart_putc(&uartStd, 'v');
	uart_putc(&uartStd, ' ');
	uart_putc(&uartStd, 'R');
	uart_putc(&uartStd, 'V');
	uart_putc(&uartStd, '3');
	uart_putc(&uartStd, '2');
	uart_putc(&uartStd, 'I');
	uart_putc(&uartStd, 'M');
	uart_putc(&uartStd, 'C');
	uart_putc(&uartStd, '@');
	uart_putc(&uartStd, '5');
	uart_putc(&uartStd, '0');
	uart_putc(&uartStd, 'M');
	uart_putc(&uartStd, 'H');
	uart_putc(&uartStd, 'z');
	uart_putc(&uartStd, '\r');
	uart_putc(&uartStd, '\n');

	uart_putc(&uartStd, '>');
	uart_putc(&uartStd, '_');
	uart_putc(&uartStd, ' ');

	gpio_dir_set(&gpioStatus, 0);
	gpio_dir_set(&gpioStatus, 1);
	gpio_dir_set(&gpioStatus, 2);

	gpio_value_set(&gpioStatus, 0);
	gpio_value_set(&gpioStatus, 1);
	gpio_value_set(&gpioStatus, 2);

	mtimer_sleep32(&mtimer, TIMER_MS(MTIMER_FREQ, 1000));
	gpio_value_clr(&gpioStatus, 0);

	mtimer_sleep32(&mtimer, TIMER_MS(MTIMER_FREQ, 250));
	gpio_value_clr(&gpioStatus, 1);

	mtimer_sleep32(&mtimer, TIMER_MS(MTIMER_FREQ, 250));
	gpio_value_clr(&gpioStatus, 2);

	uart_irq_rx_enable(&uartStd);

	while(1) {
		// 1 - 150ms - 0 - 50ms - 1 - 150ms - 0 - 1000ms
		gpio_value_set(&gpioStatus, 0);
		mtimer_sleep32(&mtimer, TIMER_MS(MTIMER_FREQ, 150));
		gpio_value_clr(&gpioStatus, 0);
		mtimer_sleep32(&mtimer, TIMER_MS(MTIMER_FREQ, 50));
		gpio_value_set(&gpioStatus, 0);
		mtimer_sleep32(&mtimer, TIMER_MS(MTIMER_FREQ, 150));
		gpio_value_clr(&gpioStatus, 0);
		mtimer_sleep32(&mtimer, TIMER_MS(MTIMER_FREQ, 1000));
		gpio_value_set(&gpioStatus, 0);
	}

	gpio_value_set(&gpioStatus, 1);
	hang();
}
