### Makefile
obj-m := rkit.o
KDIR := /lib/modules/3.13.0-35-generic/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod *.sysmvers *.order

