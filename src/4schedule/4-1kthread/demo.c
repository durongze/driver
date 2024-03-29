
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/cdev.h>
#include <linux/version.h>
#include <linux/vmalloc.h>
#include <linux/ctype.h>
#include <linux/pagemap.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/kthread.h> 

#define PDEBUG(fmt, args...) printk("[%s:%d]" fmt, __FUNCTION__, __LINE__, ## args)

static struct task_struct *simple_thread;

int threadfunc(void *data)
{
	while(1) {
		set_current_state(TASK_UNINTERRUPTIBLE);
		if (kthread_should_stop()) {
			PDEBUG(" stop...\n");
			break;
		}
		PDEBUG(" running...\n");
		schedule_timeout(HZ);
	}
	return 0;
}

void simple_cleanup_module(void)
{
	if(simple_thread) {
		kthread_stop(simple_thread);
		simple_thread = NULL;
	}
	PDEBUG(" cleanup succ.\n");
}

int simple_init_module(void)
{
	int err;
    simple_thread = kthread_run(threadfunc, NULL, "simple_thread");
    if (IS_ERR(simple_thread)) {
		PDEBUG("kthread_create failed.\n");
		err = PTR_ERR(simple_thread);
		simple_thread = NULL;
		return err;
    } else {
		PDEBUG("kthread_create succ.\n");
	}
	return 0;
}

module_init(simple_init_module);
module_exit(simple_cleanup_module);

MODULE_LICENSE("Dual BSD/GPL");
