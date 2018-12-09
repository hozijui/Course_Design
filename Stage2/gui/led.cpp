#include "led.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>

Led::Led(QObject *parent)
    :
    QObject(parent)
{
    on[0] = 0;
    on[1] = 0;
    on[2] = 0;
    on[3] = 0;

    fd = open("/dev/leds", 0);
    // cout<<"fd = "<<fd<<endl;
    if (fd < 0) {
        perror("Error : fd < 0");
    }
}

void Led::led1_on()
{
    on[0] = (~on[0]) & 1;
    ioctl(fd, on[0], 0);
}

void Led::led2_on()
{
    on[1] = (~on[1]) & 1;
    ioctl(fd, on[1], 1);
}

void Led::led3_on()
{
    on[2] = (~on[2]) & 1;
    ioctl(fd, on[2], 2);

}

void Led::led4_on()
{
    on[3] = (~on[3]) & 1;
    ioctl(fd, on[3], 3);
}

Led::~Led()
{
    //close(fd);
}
