#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/gpio.h>

#define DEVICE_NAME "mybuttons"

struct button_irq_desc{  //中断用结构体
    int irq;             //中断号
    int pin;             //GPIO引脚
    int pin_setting;     //引脚设置
    int number;          //键值
    char *name;          //按键名称
};

static struct button_irq_desc button_irqs[]={
    {IRQ_EINT8,S3C2410_GPG(0),S3C2410_GPG0_EINT8,0,"KEY1"},
    {IRQ_EINT11,S3C2410_GPG(3),S3C2410_GPG3_EINT11,1,"KEY2"},
    {IRQ_EINT13,S3C2410_GPG(5),S3C2410_GPG5_EINT13,2,"KEY3"},
    {IRQ_EINT14,S3C2410_GPG(6),S3C2410_GPG6_EINT14,3,"KEY4"},
    {IRQ_EINT15,S3C2410_GPG(7),S3C2410_GPG7_EINT15,4,"KEY5"},
    {IRQ_EINT19,S3C2410_GPG(11),S3C2410_GPG11_EINT19,5,"KEY6"}
};

static volatile char key_values[]={'0','0','0','0','0','0'};
//按键状态

static DECLARE_WAIT_QUEUE_HEAD(button_waitq);
//创建等待队列,中断产生时唤醒队列,设置中断标志,以便read函数读取键值，没有中断时,系统不会轮询按键状态，节省时钟资源

static volatile int ev_press=0;
//中断标志

//中断服务程序
static irqreturn_t buttons_interrupt(int irq,void *dev_id)
{
  struct button_irq_desc *button_irqs=(struct button_irq_desc *)dev_id;
  int down;

    down=!s3c2410_gpio_getpin(button_irqs->pin);
	//获取被按下的按键状态

    if(down!=(key_values[button_irqs->number] & 1)){
	    key_values[button_irqs->number]='0'+down;
        ev_press=1;
		//设置中断标志
        wake_up_interruptible(&button_waitq);
		//唤醒等待队列
    }
    
  return IRQ_RETVAL(IRQ_HANDLED);
}

//open函数驱动实现
static int buttons_open(struct inode *inode,struct file *file) 
{
  int i;
  int err=0;
    
    for(i=0;i<sizeof(button_irqs)/sizeof(button_irqs[0]);i++){
        if(button_irqs[i].irq<0)
            continue;
        err=request_irq(button_irqs[i].irq,buttons_interrupt,IRQ_TYPE_EDGE_BOTH,button_irqs[i].name,(void *)&button_irqs[i]);
		//注册中断,中断类型为IRQ_TYPE_EDGE_BOTH(双沿触发),能够更加有效的判断按键状态
        if(err)
            break;
    }

    if(err){ //如果出错则释放已注册的中断
        i--;
        while(i>=0){
	        if(button_irqs[i].irq<0)
		        continue;
	        disable_irq(button_irqs[i].irq);
            free_irq(button_irqs[i].irq,(void *)&button_irqs[i]);
            i--;
        }
        return -EBUSY;
    }
	
    ev_press=1; //中断注册成功,设置中断标志
    
  return 0;
}

//close函数驱动实现
static int buttons_close(struct inode *inode,struct file *file)
{
  int i;
    
    for(i=0;i<sizeof(button_irqs)/sizeof(button_irqs[0]);i++){
        if (button_irqs[i].irq<0)
            continue;
        free_irq(button_irqs[i].irq,(void *)&button_irqs[i]);
		//释放中断，并注销中断处理函数
    }

  return 0;
}

static int buttons_read(struct file *filp,char __user *buff,size_t count,loff_t *offp)
//read函数驱动实现
{
  unsigned long err;

    if(!ev_press)//中断标志为0时
	    if(filp->f_flags & O_NONBLOCK) //以非阻塞方式打开时,返回
	        return -EAGAIN;
	    else
	        wait_event_interruptible(button_waitq, ev_press); 
		//以阻塞方式打开时,挂在等待队列,等待被唤醒
    
    ev_press=0;//清中断标志
    err=copy_to_user(buff,(const void *)key_values,min(sizeof(key_values),count));
	//将一组按键状态传递到用户层

  return err?-EFAULT:min(sizeof(key_values),count);
}

static unsigned int buttons_poll(struct file *file,struct poll_table_struct *wait)
//poll函数驱动实现
{
  unsigned int mask=0;

    poll_wait(file,&button_waitq,wait);
	//将进程挂到等待队列，以便被唤醒
    if(ev_press)
        mask|=POLLIN | POLLRDNORM;

  return mask;
}

static struct file_operations dev_fops={ //文件结构体
    .owner=THIS_MODULE,                  //防止使用过程中被卸载
    .open=buttons_open,                  //open函数实现
    .release=buttons_close,              //close函数实现
    .read=buttons_read,                  //read函数实现
    .poll=buttons_poll                   //poll函数实现
};

static struct miscdevice misc={
	.minor=MISC_DYNAMIC_MINOR, //动态分配次设备号
	.name=DEVICE_NAME,         //驱动名为DEVICE_NAME,即mybuttons
	.fops=&dev_fops            //文件操作指针
}; 

static int __init dev_init(void) //模块加载时的初始化函数
{
  int ret;

    printk(KERN_ALERT"\"Mybuttons\" installed\n");
	ret = misc_register(&misc);
	//设备注册，创建设备文件,并miscdevice结构挂载到misc_list列表

  return ret;
}

static void __exit dev_exit(void) //模块卸载时的退出函数
{
    misc_deregister(&misc);
	//卸载设备，从mist_list中删除miscdevice结构，删除设备文件
    printk("\"Mybuttons\" removed\n");
}

module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Boys Next Door");
