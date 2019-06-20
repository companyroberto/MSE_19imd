ifneq ($(KERNELRELEASE),)
obj-m := myds3231.o
else
KDIR := $(HOME)/linux-kernel-labs/src/linux
all:
	$(MAKE) -C $(KDIR) M=$$PWD
endif
