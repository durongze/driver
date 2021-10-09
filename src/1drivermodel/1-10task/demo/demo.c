#include <linux/kernel.h>

//这个文件定义了linux下的task_struct数据结构
#include <linux/sched.h>   
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fdtable.h>

//里面定义了 init_task (0号进程)
#include <linux/init_task.h>  

#define PDEBUG(fmt, args...) printk( "%s:[%s:%d]" fmt, KBUILD_MODNAME, __FUNCTION__, __LINE__, ## args)

//内核模块初始化函数
static int __init print_pid(void)
{
    //指向PCB的指针
    struct task_struct *task,*p;  
    //list_head是一个双向链表,用来链接os中的所有进程,我们可以用它访问系统中的所有进程, 
    //每个pcb内部都维护一个list_head类型的tasks链表,这样就可以通过>每个进程的pcb访问所有进程了
    struct list_head *pos; 
    int count=0;//记录当前系统中的进程数目
    task=&init_task;
    //0号进程,所有进程的父进程,遍历进程链表list_head从0号进程开始
    //list_for_each用来遍历进程链表,让pos从头指向task双向链表中的每一个结点(task_struct),list_for_each是一个宏
    //list.h头文件中
    list_for_each(pos,&task->tasks)
    {
        //list_entry是一个容器宏 ,作用是:让pos从指向结构体task_struct的成员tasks,变为指向结构提task_struct本身,
        //由内部成员地址指向拥有该成员的结构体地址
        p=list_entry(pos,struct task_struct,tasks);
        count++; //++
        //现在pos指向了每一个进程的pcb,那么就可以输出pcb中的信息了
        PDEBUG("pid:%d; state:%u; prio:%d; static_prio:%d; parent'pid:%d;\n",
		    p->pid,p->__state,p->prio,p->static_prio,(p->parent)->pid);
    }

    PDEBUG("proc cnt:%d\n",count);
    return 0;
}

//内核退出函数
static void __exit pid_exit(void)
{
    PDEBUG("exiting...\n");
}

module_init(print_pid);
module_exit(pid_exit);
MODULE_LICENSE("GPL");
