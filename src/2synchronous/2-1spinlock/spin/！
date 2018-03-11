
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/spinlock.h>

MODULE_LICENSE("GPL");

#define MAJOR_NUM 224

#define init_MUTEX(sem)     sema_init(sem, 1)

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
static struct semaphore sem;
//static spinlock_t spin = SPIN_LOCK_UNLOCKED;
static DEFINE_SPINLOCK(spin);

static int sample_open(struct inode *inode, struct file *filp)
{
	/*获得自选锁*/
	spin_lock(&spin);
	
	/*临界资源访问*/
	if (sample_count)
	{
		spin_unlock(&spin);
		return - EBUSY;
	}
	sample_count++;
	
	/*释放自选锁*/
	spin_unlock(&spin);
	return 0;
}

static int sample_release(struct inode *inode, struct file *filp)
{
	sample_count--;
	return 0;
}

static ssize_t sample_read(struct file *filp, char *buf, size_t len, loff_t*off)
{
	return 0;
}

static ssize_t sample_write(struct file *filp, const char *buf, size_t len,loff_t *off)
{
	return 0;
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
		init_MUTEX(&sem);
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
