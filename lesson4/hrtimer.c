#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Piskun");
MODULE_DESCRIPTION("Test timers");
MODULE_VERSION("0.22");

#define MS_TO_NS(ms)    (unsigned long)((ms * (unsigned long)1000 * (unsigned long)1000))

static int count = 0;
static ktime_t ktPrev;

static void staticTimerExpiredCb(struct timer_list *pTmList);
static enum hrtimer_restart myHrTimCallback(struct hrtimer *timer);

static DEFINE_TIMER(myStaticTim, staticTimerExpiredCb);
static struct hrtimer myHrTim;

static void staticTimerExpiredCb(struct timer_list *pTmList)
{
    printk(KERN_INFO "Timer expired!\n");
    if (++count <= 5)
    {
        printk(KERN_INFO "Timer rescheduled; cnt: %d!\n", count);
        mod_timer(&myStaticTim, jiffies + HZ * 2);
    }
}

static enum hrtimer_restart myHrTimCallback(struct hrtimer *timer)
{
    ktime_t kt;

    kt = timer->base->get_time();

    printk(KERN_INFO "HR: %lld; diff: %lld\n", kt, kt - ktPrev);
    ktPrev = kt;
    hrtimer_forward_now(timer, ns_to_ktime(MS_TO_NS(1000)));

    return HRTIMER_RESTART;
}

static int __init lesson4_init(void)
{
    ktime_t kt;
    myStaticTim.expires = jiffies + 2 * HZ;
    add_timer(&myStaticTim);

    kt = ktime_set(0, MS_TO_NS(1000));
    hrtimer_init(&myHrTim, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    myHrTim.function = myHrTimCallback;
    hrtimer_start(&myHrTim, kt, HRTIMER_MODE_REL);

    printk(KERN_INFO "Timers have been set up\n");

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

    status = hrtimer_cancel(&myHrTim);
    if (status < 0)
    {
        printk(KERN_ERR "hrtimer_cancel() failed; err: %d\n", status);

        return;
    }

    printk(KERN_INFO "timers deleted\n");

    printk(KERN_INFO "module exit\n");
}

module_init(lesson4_init);
module_exit(lesson4_exit);
