#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Piskun");
MODULE_DESCRIPTION("Test IRQ handling");
MODULE_VERSION("0.22");

#define GPIO2_8		(32 * 2 + 8)

static unsigned int uniq_dev_id;

static irqreturn_t my_quick_irq_handler(int irq, void *p_param)
{
    printk(KERN_INFO "In quick IRQ handler\n");

    return IRQ_HANDLED;
}

static irqreturn_t my_main_irq_handler(int irq, void *p_param)
{
    printk(KERN_INFO "In main IRQ handler\n");

    return IRQ_HANDLED;
}


static int __init irq_hndl_module_init(void)
{
    int irq;
    int status;

    printk(KERN_INFO "Module init\n");

    status = gpio_request(GPIO2_8, "user_boot");
    if (status < 0)
    {
        printk(KERN_INFO "gpio_request() failed; error: %d\n", status);
    }

    status = gpio_direction_input(GPIO2_8);
    if (status < 0)
    {
        printk(KERN_INFO "gpio_direction_input() failed; error: %d\n", status);
    }

    irq = gpio_to_irq(GPIO2_8);

    printk(KERN_INFO "irq: %d\n", irq);

    status = request_threaded_irq(
            irq, 
            my_main_irq_handler,
            my_quick_irq_handler,
            IRQF_SHARED,
            "lesson6",
            &uniq_dev_id);

    if (status < 0)
    {
        printk(KERN_INFO "request_threaded_irq() failed; error: %d\n", status);
    }

    return 0;
}

static void __exit irq_hndl_module_exit(void)
{
    printk(KERN_INFO "Module exit\n");
}

module_init(irq_hndl_module_init);
module_exit(irq_hndl_module_exit);
