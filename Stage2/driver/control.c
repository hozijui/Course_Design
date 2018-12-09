#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/ioctl.h>  

int set(int on,int led_on)
{
    int fd = open("/dev/myleds",0);
    if(fd<0)
    {  
        perror("open device leds");  
        exit(1);  
    }  
    ioctl(fd,on,led_on);  
    close(fd);  
}
