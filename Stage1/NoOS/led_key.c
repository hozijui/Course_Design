#include "2440_GPX.h"

int main()
{
    GPXXOUT(GPBCON,5);GPXXOUT(GPBCON,6);GPXXOUT(GPBCON,7);GPXXOUT(GPBCON,8);
    GPXXIN(GPGCON,0);GPXXIN(GPGCON,3);GPXXIN(GPGCON,5);GPXXIN(GPGCON,6);

    while(1)
    {
        unsigned long dwData;
        dwData=GPGDAT;

        /*K1*/
        if(dwData & (1<<0))
	        GPBDAT |= (1<<5);
        else
            GPBDAT &= ~(1<<5);

	    /*K2*/
        if(dwData & (1<<3))
            GPBDAT |= (1<<6);
        else
            GPBDAT &= ~(1<<6);

	    /*K3*/
        if(dwData & (1<<5))
            GPBDAT |= (1<<7);
        else
            GPBDAT &= ~(1<<7);

	    /*K4*/
        if(dwData & (1<<6))
            GPBDAT |= (1<<8);
        else
            GPBDAT &= ~(1<<8);
    } 
    return 0;
}
