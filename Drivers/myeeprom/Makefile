ifneq ($(KERNELRELEASE),)
obj-m := myeeprom.o
else
KDIR := $(HOME)/linux-kernel-labs/src/linux
all:
	$(MAKE) -C $(KDIR) M=$$PWD
endif
