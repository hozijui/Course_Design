led_on_c.bin: crt0.S led_key.c
	arm-linux-gcc -g -c -o crt0.o crt0.S
	arm-linux-gcc -g -c -o led_key.o led_key.c
	arm-linux-ld -Ttext 0x0000000 -g crt0.o led_key.o -o led_key_elf
	arm-linux-objcopy -O binary -S led_key_elf led_key.bin
clean:
	rm -f *.o led_key_elf
