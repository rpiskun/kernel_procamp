#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/timekeeping.h>
#include <linux/ktime.h>
#include <linux/workqueue.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Piskun");
MODULE_DESCRIPTION("Test tasklet");
MODULE_VERSION("0.22");

static void lo_prio_tasklet_handler(unsigned long data);
static void hi_prio_tasklet_handler(unsigned long data);
static void timer_expired_cb(struct timer_list *p_tim_list);
static void my_work_handler(struct work_struct *work);

static int param = 5;

static ktime_t lo_kt;
static ktime_t hi_kt;
static struct work_struct my_work;

static DECLARE_TASKLET(lo_prio_tasklet, lo_prio_tasklet_handler, (unsigned long)&param);
static DECLARE_TASKLET(hi_prio_tasklet, hi_prio_tasklet_handler, (unsigned long)&param);
static DEFINE_TIMER(tim, timer_expired_cb);

static void timer_expired_cb(struct timer_list *p_tim_list)
{
    unsigned long my_jiffies;
    ktime_t kt;

    my_jiffies = jiffies;
    kt = ktime_get();

    tasklet_schedule(&lo_prio_tasklet);
    (void)queue_work(system_wq, &my_work);
    printk(KERN_INFO "timer_expired_cb()       : jiffies: %lu; kt: %lld\n", my_jiffies, kt);
    tasklet_hi_schedule(&hi_prio_tasklet);
}

static void my_work_handler(struct work_struct *work)
{
    unsigned long my_jiffies;
    ktime_t kt;
    (void)work;

    my_jiffies = jiffies;
    kt = ktime_get();

    printk(KERN_INFO "my_work_handler()        : jiffies: %lu; kt: %lld\n", my_jiffies, kt);
    printk(KERN_INFO "my_work_handler(): enter sleep...\n");
    msleep(10);
    printk(KERN_INFO "my_work_handler(): exit sleep\n");

}

static void lo_prio_tasklet_handler(unsigned long data)
{
    unsigned long my_jiffies;
    (void)data;

    my_jiffies = jiffies;
    lo_kt = ktime_get();

    if (hi_kt > 0 && lo_kt > 0)
    {
        printk(KERN_INFO "lo_prio_tasklet_handler(): jiffies: %lu; kt: %lld; diff: %lld\n", my_jiffies, lo_kt, (lo_kt - hi_kt));
    }
    else
    {
        printk(KERN_INFO "lo_prio_tasklet_handler(): jiffies: %lu; kt: %lld; diff: ---\n", my_jiffies, lo_kt);
    }

    // printk(KERN_INFO "lo_prio_tasklet_handler(): enter sleep...\n");
    /* Calling msleep() in tasklet causes BUG: scheduling while atomic: ksoftirqd */
    // msleep(10);
    // printk(KERN_INFO "lo_prio_tasklet_handler(): exit sleep\n");
}

static void hi_prio_tasklet_handler(unsigned long data)
{
    unsigned long my_jiffies;
    (void)data;

    my_jiffies = jiffies;
    hi_kt = ktime_get();

    if (hi_kt > 0 && lo_kt > 0)
    {
        printk(KERN_INFO "hi_prio_tasklet_handler(): jiffies: %lu; kt: %lld; diff: %lld\n", my_jiffies, hi_kt, (lo_kt - hi_kt));
    }
    else
    {
        printk(KERN_INFO "hi_prio_tasklet_handler(): jiffies: %lu; kt: %lld; diff: ---\n", my_jiffies, hi_kt);
    }
}

static int __init tasklet_module_init(void)
{
    printk(KERN_INFO "Module init\n");

    INIT_WORK(&my_work, my_work_handler);

    tim.expires = jiffies + 1 * HZ;
    add_timer(&tim);

    return 0;
}

static void __exit tasklet_module_exit(void)
{
    printk(KERN_INFO "Module exit\n");
}

module_init(tasklet_module_init);
module_exit(tasklet_module_exit);
