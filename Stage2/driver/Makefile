PWD = $(shell pwd)  
KDIR =kernel/linux-2.6.32.2/
obj-m:= myleds.o mybuttons.o
all:  
	$(MAKE) -C $(KDIR) M=$(PWD) CONFIG_DEBUG_SECTION_MISMATCH=y  
clean:  
	rm -rf *.o *~core.depend. *.cmd *.mod.c .tmp_versions  
	rm -rf *.order Module.*  
active:  
	echo -e "$(MAKE) \n"  
	$(MAKE) -C $(KDIR) M=$(PWD)
