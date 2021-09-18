#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/slab.h>

#define PDEBUG(fmt, args...) printk("[%s:%d]" fmt, __FUNCTION__, __LINE__, ## args)

struct simplelist
{
	struct list_head node;
	char buffer;
};

LIST_HEAD(mylist);

struct simplelist *create_list_node(void)
{
	struct simplelist *p;
	p = (struct simplelist *)kmalloc(sizeof(struct simplelist),GFP_KERNEL);
	return p;
}

int create_simple_list(int size)
{
	int i=0;
	struct simplelist *p;
	for(i = 0; i < size; i++) {
		p = create_list_node();
		p->buffer = 0x31+i;
		list_add_tail(&p->node,&mylist);
	}
	return 0;
}

void each_simple_list(void)
{
	struct simplelist* slistp;
	list_for_each_entry(slistp, &mylist, node) {
		PDEBUG("find a list buffer is %c\n", slistp->buffer);
	}
	return ;
}

static int demo_module_init(void)
{
	PDEBUG("demo_module_init\n");
	create_simple_list(5);
	each_simple_list();
	return 0;
}

static void demo_module_exit(void)
{
	PDEBUG("demo_module_exit\n");
}

module_init(demo_module_init);
module_exit(demo_module_exit);

MODULE_DESCRIPTION("simple list module");
MODULE_LICENSE("GPL");

