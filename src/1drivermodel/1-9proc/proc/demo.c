#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/slab.h>

static int itype=0;
module_param(itype, int, 0);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("PROC FILE DEMO");

#define MAX_LENGTH   500
#define MAX_simple_LENGTH 1024
#define PDEBUG(fmt, args...) printk( "%s:[%s:%d]" fmt, KBUILD_MODNAME, __FUNCTION__, __LINE__, ## args)

static struct proc_dir_entry *proc_entry;
static char *simple_buffer;

void show_task_struct(struct task_struct *p, char *page, int page_size)
{
	int size;
	char state;
	size = sprintf(page, "%5s%7s%7s%7s%7s%7s%7s  %s\n\n",
		"PID","UID","PRIO","POLICY", "STATE","UTIME","STIME","COMMAND");
	
	for_each_process(p) {
		switch((int)p->__state) {
			case -1: state='Z'; break;
			case 0: state='R'; break;
			default: state='S'; break;
		}
		size += sprintf(page + size, "%5d%7d%7d%7d%7c%7d%7d  %s\n",
			(int)p->pid, p->cred->uid.val, (int)p->rt_priority, (int)p->policy,
			state, (int)p->utime, (int)p->stime, p->comm);
	}

}

ssize_t simple_read_(struct file *filp, char *buff, size_t len,  loff_t *offset)
{
	return 0;
}

int simple_read( char *page, char **start, off_t off,int count, int *eof, void *data )
{
	int size = 0;
	struct task_struct *p = NULL;
	show_task_struct(p, page, count);
	return (size);
}

ssize_t simple_write_(struct file *filp, const char *buff, size_t len,  loff_t *offset)
{
	return 0;
}

ssize_t simple_write( struct file *filp, const char __user *buff,
					 unsigned long len, void *data )
{
	if(len>MAX_LENGTH)len=MAX_LENGTH;
	if (copy_from_user(simple_buffer, buff, len )) {
		return -EFAULT;
	}
	simple_buffer[len] = 0;
	PDEBUG(KERN_INFO "simple_write: %s\n",simple_buffer);
	return len;
}

/* 
* This function decides whether to allow an operation
* (return zero) or not allow it (return a non-zero
* which indicates why it is not allowed).
*
* The operation can be one of the following values:
* 0 - Execute (run the "file" - meaningless in our case)
* 2 - Write (input to the kernel module)
* 4 - Read (output from the kernel module)
*
* This is the real function that checks file
* permissions. The permissions returned by ls -l are
* for referece only, and can be overridden here.
*/

// static int simple_permission(struct inode *inode, int op, struct nameidata *foo)
static int simple_permission(struct user_namespace *ns, struct inode *node, int op)
{
	PDEBUG(KERN_INFO "simple_permission op %d\n",op);
	if(itype == op) {
		return -EACCES;
	}
	return 0;
}

static struct inode_operations simple_inode_operations = {
	.permission = simple_permission,
};

struct file_operations proc_fops=
{
    .read=simple_read_,
    .write=simple_write_,
    .owner=THIS_MODULE,
};

const struct proc_ops proc_fops_ =
{
    .proc_read=simple_read_,
    .proc_write=simple_write_,
};

int init_simple_module( void )
{
	int ret = 0;
	simple_buffer = (char *)vmalloc( MAX_simple_LENGTH );
	if (!simple_buffer) 
	{
		ret = -ENOMEM;
	} 
	else 
	{
		memset( simple_buffer, 0, MAX_LENGTH );
#if 0
		proc_entry = create_proc_entry( "demo", 0644, NULL );
		if (proc_entry == NULL)
		{
			ret = -ENOMEM;
			vfree(simple_buffer);
			PDEBUG(KERN_INFO "demo: Couldn't create proc entry\n");
			
		} 
		else
		{
			proc_entry->proc_iops = &simple_inode_operations;
			proc_entry->read_proc = simple_read;
			proc_entry->write_proc = simple_write;
			proc_entry->owner = THIS_MODULE;
			PDEBUG(KERN_INFO "demo: Module loaded.\n");
		}
#else
    proc_entry = proc_create( "demo", 0644, NULL, &proc_fops_);
    if (NULL == proc_entry)
    {
		PDEBUG(KERN_INFO "proc_create.\n");
    }
#endif
	}
	return ret;
}


void cleanup_simple_module( void )
{
	remove_proc_entry("demo", proc_entry);
	vfree(simple_buffer);
	PDEBUG(KERN_INFO "demo: Module unloaded.\n");
}


module_init( init_simple_module );
module_exit( cleanup_simple_module );

