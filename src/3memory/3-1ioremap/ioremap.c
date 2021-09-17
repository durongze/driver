
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
#include <linux/mm.h>
#include <asm/io.h>
#include <linux/slab.h>

#define ONE_K 10
#define ONE_M (ONE_K * ONE_K)
#define MMAP_DRV "mmapdrv"
#define PDEBUG(fmt, args...) printk("[%s:%d]" fmt, __FUNCTION__, __LINE__, ## args)

static int mem_start = 101, mem_size = 10;
static char *reserve_virt_addr;
static int major;

int mmapdrv_open(struct inode *inode, struct file *file)
{
	PDEBUG("%s\n", MMAP_DRV);
	return (0);
}

int mmapdrv_release(struct inode *inode, struct file *file)
{
	PDEBUG("%s\n", MMAP_DRV);
	return (0);
}

static struct file_operations mmapdrv_fops =
{
	owner: THIS_MODULE, 
	open: mmapdrv_open, 
	release:mmapdrv_release,
};

int init_reserve_virt_addr(char *reserve_virt_addr,  int mem_size)
{
	int i, step;
	if (reserve_virt_addr == NULL) {
		return -1;	
	}
	for (i = 0, step = 4; i + step <= mem_size * ONE_M; i += step) {
		reserve_virt_addr[i] = 'a';
		reserve_virt_addr[i + 1] = 'b';
		reserve_virt_addr[i + 2] = 'c';
		reserve_virt_addr[i + 3] = 'd';
	}
	return 0;
}

int init_moduleiomap(void)
{
	major = register_chrdev(0, MMAP_DRV, &mmapdrv_fops);
	PDEBUG("register_chrdev %s:%d\n", MMAP_DRV, major);
	if (major < 0) {
		return ( - EIO);
	}
	PDEBUG("high memory physical address 0x%lldM\n", virt_to_phys(high_memory) / ONE_M);
	
	reserve_virt_addr = ioremap(mem_start * ONE_M, mem_size * ONE_M);
	PDEBUG("reserve_virt_addr = 0x%lx, size:%d\n", (unsigned long)reserve_virt_addr, mem_size);
	if (reserve_virt_addr == NULL) {
		unregister_chrdev(major, MMAP_DRV);
		return - ENODEV;
	} else {
		init_reserve_virt_addr(reserve_virt_addr, mem_size);
	}
	return 0;
}

/* remove the module */
void cleanup_moduleiomap(void)
{
	if (reserve_virt_addr) {
		iounmap(reserve_virt_addr);
	}
	PDEBUG("unregister_chrdev %s:%d\n", MMAP_DRV, major);
	unregister_chrdev(major, MMAP_DRV);
	return ;
}

module_init(init_moduleiomap);
module_exit(cleanup_moduleiomap);

MODULE_LICENSE("Dual BSD/GPL");
