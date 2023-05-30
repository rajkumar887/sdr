obj-m += rot_switch.o
KDIR := /home/vijay/ti-processor-sdk-linux-am57xx-evm-04.01.00.06/board-support/linux-4.9.41+gitAUTOINC+e3a80a1c5c-ge3a80a1c5c
PWD := $(shell pwd)
ARCH:=arm
COMPILER:=arm-linux-gnueabihf-
default:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(COMPILER) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) clean
