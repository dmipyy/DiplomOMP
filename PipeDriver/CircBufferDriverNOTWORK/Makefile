obj-m += driver.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test:
	#an error in case the module isn't loaded
	-sudo rmmod driver
	#Clear the kernel log without echo
	sudo dmesg -C
	#Insert the module
	sudo insmod driver.ko
	#Display the kernel log
	sudo dmesg
	gcc ./client.c
	#sudo ./a.out
