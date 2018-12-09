#define GPIOBASE 0x56000000
#define GPXCON_ADDR(x) (*(volatile unsigned long *)(GPIOBASE + x))
#define GPXDAT_ADDR(x) (*(volatile unsigned long *)(GPIOBASE + x + 4))

#define GPB 0x10
#define GPG 0x60

/*GPXCON配置寄存器 X代表不同的类型*/ 
#define GPBCON GPXCON_ADDR(GPB)
#define GPGCON GPXCON_ADDR(GPG)

/*GPXDAT数据寄存器*/
#define GPBDAT GPXDAT_ADDR(GPB)
#define GPGDAT GPXDAT_ADDR(GPG)

#define GPXXOUT(GPXCON,x) (GPXCON |=  (0x1 << (2 * x)))
/******************************************************************************
  设置GPIO控制端口GPXCON的x位为输入，GPXCON必须为GPBCON~GPJCON，
  x必须<=16
把对应bit变为00即设置为输出
 ******************************************************************************/
#define GPXXIN(GPXCON,x) (GPXCON &= ((GPXCON & ~(0x3 << (2 * x)))))
/********************************************************************************
 设置GPIO端口GPXDAT的x位为高电平，GPXCON必须为GPBCON~GPJCON，
 x视不同的BANK拥有的I/O口的个数而定
 ********************************************************************************/
