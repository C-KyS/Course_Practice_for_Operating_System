obj-m := showthread.o  # 内核模块名

KDIR := /lib/modules/$(shell uname -r)/build # 内核源码路径
PWD := $(shell pwd) # 当前路径

default:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
