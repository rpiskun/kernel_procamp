#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Piskun");
MODULE_DESCRIPTION("Test timers");
MODULE_VERSION("0.22");

static void staticTimerExpiredCb(struct timer_list *pTmList);

// DEFINE_TIMER(myStaticTim, staticTimerExpiredCb);
static struct timer_list myStaticTim;

static void staticTimerExpiredCb(struct timer_list *pTmList)
{
    printk(KERN_INFO "Timer expired! flags: %d!\n", pTmList->flags);
}

static int __init lesson4_init(void)
{
    // myStaticTim.flags = 5;
    timer_setup(&myStaticTim, staticTimerExpiredCb, 5);

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
}

module_init(lesson4_init);
module_exit(lesson4_exit);