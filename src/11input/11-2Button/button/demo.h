
#ifndef _button_H_
#define _button_H_

#include <linux/ioctl.h> /* needed for the _IOW etc stuff used later */

/********************************************************
 * Macros to help debugging
 ********************************************************/
#undef PDEBUG             /* undef it, just in case */
#ifdef button_DEBUG
#ifdef __KERNEL__
#    define PDEBUG(fmt, args...) printk( "%s:[%s:%d]" fmt, KBUILD_MODNAME, __FUNCTION__, __LINE__, ## args)
#else//usr space
#    define PDEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
#endif
#else
#  define PDEBUG(fmt, args...) /* not debugging: nothing */
#endif

#undef PDEBUGG
#define PDEBUGG(fmt, args...) /* nothing: it's a placeholder */


//设备结构
struct button_dev 
{
	struct cdev cdev;	  /* Char device structure		*/
};

//函数申明
ssize_t button_read(struct file *filp, char __user *buf, size_t count,
                   loff_t *f_pos);
ssize_t button_write(struct file *filp, const char __user *buf, size_t count,
                    loff_t *f_pos);
loff_t  button_llseek(struct file *filp, loff_t off, int whence);
int     button_ioctl(struct inode *inode, struct file *filp,
                    unsigned int cmd, unsigned long arg);


#endif /* _button_H_ */
