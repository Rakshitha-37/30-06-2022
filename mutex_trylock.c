#include <linux/module.h> 
#include <linux/kthread.h>	//thread
#include <linux/delay.h> 
#include<linux/mutex.h>

MODULE_LICENSE("GPL");

struct mutex my_mutex;

int  my_thread_handle(void *p);

int count =0;

struct task_struct *char_thread;
struct task_struct *char_thread2;

int my_thread_handle(void *p)
{
        int lock_available = 0;
	while(!kthread_should_stop())
	{
	        lock_available = mutex_trylock(&my_mutex);
	        if(lock_available == 0) 
	        {
		      printk("My thread function %d",count++);
		      mutex_unlock(&my_mutex);
		      msleep(1000);
		}
		else
		{
		      printk(KERN_INFO "Lock is not available\n");
		}
		
	}
	return 0;
}


static int __init my_thread_driver_init(void)
{
	
	printk("hello world\n");

        mutex_init(&my_mutex);
	char_thread = kthread_run(my_thread_handle, NULL, "thread1");

	if(char_thread)
	{
		printk("thread created successfully");
	}
	else
	{
		printk("thread created failed");
		 
	}
	char_thread2 = kthread_run(my_thread_handle, NULL, "thread2");

	if(char_thread2)
	{
		printk("thread2 created successfully");
	}
	else
	{
		printk("thread created failed");
		 
	}
	printk("my thread function %d",count);
	return 0;
}

static void __exit my_therad_driver_exit(void)
{

	printk("exit hello world\n");

	kthread_stop(char_thread);
	kthread_stop(char_thread2);
}

module_init(my_thread_driver_init);
module_exit(my_therad_driver_exit);
