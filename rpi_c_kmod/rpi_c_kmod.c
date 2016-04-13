/* Adopted from:
 *
 * Basic kernel module using some GPIOs to drive LEDs.
 *
 * Author:
 * 	Stefan Wendler (devnull@kaltpost.de)
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#define PIN 17
#define POUT 18

static struct gpio gpiomod_pins[] = {
		{ PIN, GPIOF_IN, "trigger" },
		{ POUT, GPIOF_OUT_INIT_HIGH, "response" },
};

static int gpiomod_irq = -1;

static irqreturn_t gpiomod_isr(int irq, void* data)
{
	if(irq == gpiomod_irq) {
		int v = gpio_get_value(PIN);
		gpio_set_value(POUT, v);
	}

	return IRQ_HANDLED;
}

static int __init gpiomod_init(void)
{
	int ret = 0;

	printk(KERN_INFO "%s\n", __func__);

	ret = gpio_request_array(gpiomod_pins, ARRAY_SIZE(gpiomod_pins));
	if(ret) {
		printk(KERN_ERR "Unable to request GPIOs: %d\n", ret);
		return ret;
	}

	ret = gpio_to_irq(PIN);
	if(ret < 0) {
		printk(KERN_ERR "Unable to get IRQ: %d\n", ret);
		goto fail1;
	}

	gpiomod_irq = ret;

	printk(KERN_INFO "Successfully requested IRQ #%d\n", gpiomod_irq);

	ret = request_irq(gpiomod_irq, gpiomod_isr,
			IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING,
			"gpiomod trigger", NULL);
	if(ret) {
		printk(KERN_ERR "Unable to request IRQ: %d\n", ret);
	}

	return ret;

fail1:
	gpio_free_array(gpiomod_pins, ARRAY_SIZE(gpiomod_pins));

	return ret;
}

static void __exit gpiomod_exit(void)
{
	printk(KERN_INFO "%s\n", __func__);

	free_irq(gpiomod_irq, NULL);

	gpio_free_array(gpiomod_pins, ARRAY_SIZE(gpiomod_pins));
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stefan Wendler");
MODULE_DESCRIPTION("Basic Linux Kernel module using GPIOs to drive LEDs");

module_init(gpiomod_init);
module_exit(gpiomod_exit);
