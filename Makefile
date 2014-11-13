ifeq (${KDIR},)
KDIR := /lib/modules/`uname -r`/build
endif
pwd = $(shell pwd)

ifneq (${LINUXINCLUDE},)
# If we being invoked from kbuild, prepend the proper include paths
LINUXINCLUDE := \
	-I${M}/include \
	${LINUXINCLUDE}
endif

helloworld-objs += helloworld_main.o
obj-m += helloworld.o

all:
	@./tools/check-syntax save-kdir $(KDIR) .kdir
	+make -C $(KDIR) M=$(pwd) modules

clean:
	+make -C $(KDIR) M=$(pwd) clean

check-syntax:
	tools/check-syntax $(CHK_SOURCES)
