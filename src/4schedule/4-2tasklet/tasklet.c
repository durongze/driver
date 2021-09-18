#include <linux/version.h>
#include <linux/module.h>
#include <linux/interrupt.h> 
#include <linux/time.h>

#define PDEBUG(fmt, args...) printk( "%s:[%s:%d]:" fmt, KBUILD_MODNAME, __FUNCTION__, __LINE__, ## args)

void tasklet_function(struct tasklet_struct *arg);

DECLARE_TASKLET(test_tasklet, tasklet_function);

void tasklet_pfunc(unsigned long data)
{
	PDEBUG("data:%ld\n", data);
}

void tasklet_function(struct tasklet_struct *data)
{
	PDEBUG("next:%p,state:%lu,count:%d,func:%p,data:%ld\n",
		 data->next, data->state, data->count.counter, data->func, data->data);
}

int init_module_task(void) 
{
	tasklet_schedule(&test_tasklet);
	PDEBUG("tasklet_schedule ");
	return 0;
}

void cleanup_moduletask(void)
{
	PDEBUG("  ");
	return ;
}

module_init(init_module_task);
module_exit(cleanup_moduletask);

MODULE_AUTHOR("fgjnew@163.com");
MODULE_LICENSE("Dual BSD/GPL");
