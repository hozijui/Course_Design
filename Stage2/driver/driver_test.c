#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>

int main()
{
  int i;
  int led_fd,button_fd;
  int leds[4]={0,0,0,0};

    led_fd=open("/dev/myleds",0);
    button_fd=open("/dev/mybuttons",0);

    for(i=0;i<4;i++)
      ioctl(led_fd,leds[i],i);

    while(1){
      char pressed[6];
      read(button_fd,pressed,sizeof pressed);
      for(i=0;i<4;i++){
        leds[i]=(leds[i]+pressed[i])%2;
        ioctl(led_fd,leds[i],i);
	    }
    }      

    close(led_fd);
    close(button_fd);
  return 0;
}
