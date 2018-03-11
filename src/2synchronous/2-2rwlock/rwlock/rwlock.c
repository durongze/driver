
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/spinlock.h>

MODULE_LICENSE("GPL");

#define MAJOR_NUM 224

//rwlock_t lock = RW_LOCK_UNLOCKED;
DEFINE_RWLOCK(lock);
char demoBuffer[256];

static ssize_t sample_read(struct file *, char *, size_t, loff_t*);
static ssize_t sample_write(struct file *, const char *, size_t, loff_t*);
static int sample_open(struct inode *inode, struct file *filp);
static int sample_release(struct inode *inode, struct file *filp);

struct file_operations sample_fops =
{
	read: sample_read, 
	write: sample_write, 
	open: sample_open,
	release:sample_release,
};

//static int sample_var = 0;
static int sample_count = 0;

static int sample_open(struct inode *inode, struct file *filp)
{
	sample_count++;
	return 0;
}

static int sample_release(struct inode *inode, struct file *filp)
{
	sample_count--;
	return 0;
}

static ssize_t sample_read(struct file *filp, char *buf, size_t len, loff_t*off)
{
	int count=len;
	read_lock(&lock);
    if (copy_to_user(buf,demoBuffer,count))
	{
	   count=-EFAULT;
	}
	read_unlock(&lock);
	return count;
}

static ssize_t sample_write(struct file *filp, const char *buf, size_t len,loff_t *off)
{
	int count=len;
	write_lock(&lock);
    if (copy_from_user(demoBuffer, buf, count))
	{
		count = -EFAULT;
	}
	write_unlock(&lock);
	return count;
}


static int __init sample_init(void)
{
	int ret;
	/*注册设备驱动*/
	ret = register_chrdev(MAJOR_NUM, "chardev", &sample_fops);
	if (ret)
	{
		printk("chardev register failure\n");
	}
	else
	{
		printk("chardev register success\n");
	}
	return ret;
}

static void __exit sample_exit(void)
{
	/*注销设备驱动*/
	unregister_chrdev(MAJOR_NUM, "chardev");
}

module_init(sample_init);
module_exit(sample_exit); 
