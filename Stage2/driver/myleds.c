#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <asm/irq.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/pci.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <asm/unistd.h>


#define DEVICE_NAME "myleds"

static unsigned long led_table[]={ //定义led的GPIO引脚
	S3C2410_GPB(5),
	S3C2410_GPB(6),
	S3C2410_GPB(7),
	S3C2410_GPB(8)
};

static unsigned int led_cfg_table[]={ //设置led引脚功能为输出
	S3C2410_GPIO_OUTPUT,
	S3C2410_GPIO_OUTPUT,
	S3C2410_GPIO_OUTPUT,
	S3C2410_GPIO_OUTPUT
};


//ioctl函数的驱动实现
static int leds_ioctl(struct inode *inode,struct file *file,unsigned int cmd,unsigned long arg)  
{
	if(cmd==0 || cmd==1){
		if(arg>4) return -EINVAL;
		    s3c2410_gpio_setpin(led_table[arg],!cmd); //设置相应的引脚状态
		return 0;
	}
	else
		return -EINVAL;
}

static struct file_operations dev_fops={ //文件结构体
	.owner=THIS_MODULE,                  //防止使用过程中被卸载
	.ioctl=leds_ioctl                    //ioctl函数实现
};

static struct miscdevice misc={ //混杂设备结构体
	.minor=MISC_DYNAMIC_MINOR,  //动态分配次设备号
	.name=DEVICE_NAME,          //驱动名为DEVICE_NAME,即myleds
	.fops=&dev_fops             //文件操作指针
};

static int __init dev_init(void) //模块加载时的初始化函数
{
  int ret,i;
    
    printk(KERN_ALERT"\"Myleds\" installed\n");

	for (i=0;i<4;i++){
		s3c2410_gpio_cfgpin(led_table[i],led_cfg_table[i]);
		//配置led引脚的功能
		s3c2410_gpio_setpin(led_table[i],0);
		//初始化led状态为点亮
	}

	ret=misc_register(&misc);  
	//设备注册，创建设备文件,并miscdevice结构挂载到misc_list列表
	printk ("\"Myleds\" initialized\n");

  return ret;
}

static void __exit dev_exit(void) //模块卸载时的退出函数
{
	misc_deregister(&misc);
	//卸载设备，从mist_list中删除miscdevice结构，删除设备文件
	printk("\"Myleds\" removed\n");
}

module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Boys Next Door");
