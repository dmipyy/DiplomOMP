obj-m += randomMouse.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test:
	#an error in case the module isn't loaded
	-sudo rmmod mouse
	#Clear the kernel log without echo
	sudo dmesg -C
	#Insert the module
	sudo insmod randomMouse.ko
	#Display the kernel log
	sudo dmesg
