#include "linux/init.h"
#include "linux/module.h"
#include "linux/kernel.h"
#include "linux/sched/signal.h"
#include "linux/sched.h"

MODULE_LICENSE("GPL");

// 初始化函数
static int __init show_all_kernel_thread_init(void)
{
    struct task_struct *p;
    printk("%-20s%-6s%-6s%-6s%-6s", "Name", "PID", "State", "Prio", "PPID");
    printk("--------------------------------------------");
    
    // 遍历进程链表中的所有进程
    for_each_process(p)
    {
        // p最开始指向进程链表中第一个进程，随着循环不断进行p也不断后移直至链表尾
        if (p->mm == NULL)
        {
            // 打印进程p的相关信息
            char state_char = task_state_to_char(p);
            printk("%-20s%-6d%-6c%-6d%-6d", p->comm, p->pid, state_char, p->prio,
                   p->parent->pid);
        }
    }
    
    return 0;
}

// 清理函数
static void __exit show_all_kernel_thread_exit(void)
{
    printk("[ShowAllKernelThread] Module Uninstalled.\n");
}

module_init(show_all_kernel_thread_init);
module_exit(show_all_kernel_thread_exit);
