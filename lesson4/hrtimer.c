#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Piskun");
MODULE_DESCRIPTION("Test timers");
MODULE_VERSION("0.22");

static int count = 0;

static void staticTimerExpiredCb(struct timer_list *pTmList);

static DEFINE_TIMER(myStaticTim, staticTimerExpiredCb);

static void staticTimerExpiredCb(struct timer_list *pTmList)
{
    printk(KERN_INFO "Timer expired!\n");
    if (++count <= 5)
    {
        printk(KERN_INFO "Timer rescheduled; cnt: %d!\n", count);
        mod_timer(&myStaticTim, jiffies + HZ * 2);
    }
}

static int __init lesson4_init(void)
{
    myStaticTim.expires = jiffies + 2 * HZ;
    add_timer(&myStaticTim);

    printk(KERN_INFO "Timer has been set up\n");

    return 0;
}

static void __exit lesson4_exit(void)
{
    int status = del_timer_sync(&myStaticTim);
    if (status < 0)
    {
        printk(KERN_ERR "del_timer_sync() failed; err: %d\n", status);

        return;
    }

    printk(KERN_INFO "timer deleted\n");

    printk(KERN_INFO "module exit\n");
}

module_init(lesson4_init);
module_exit(lesson4_exit);