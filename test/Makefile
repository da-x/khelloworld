ifeq (${KDIR},)
KDIR := /lib/modules/`uname -r`/build
endif
pwd = `pwd`

ifneq (${LINUXINCLUDE},)
# If we being invoked from kbuild, prepend the proper include paths
LINUXINCLUDE := \
	-I${M}/../include \
	${LINUXINCLUDE}
endif

helloworld-test-objs += helloworld_test.o
obj-m += helloworld-test.o

all: helloworld-test
	+make -C $(KDIR) \
		M=$(pwd) \
		KBUILD_EXTRA_SYMBOLS=$(pwd)/../Module.symvers \
		modules

clean:
	+make -C $(KDIR) M=$(pwd) clean
	rm -f helloworld-test

USER_CXXFLAGS=-O2 -Wall -fno-strict-aliasing -I../include/uapi -std=c++11

check-syntax:
	@if [[ $(CHK_SOURCES) =~ \.cpp$$ ]];  \
            then g++ -fsyntax-only ${USER_CXXFLAGS} $(CHK_SOURCES) -c ; \
            else ../tools/check-syntax $(CHK_SOURCES) ; \
        fi

helloworld-test: helloworld-test.cpp ../include/uapi/linux/helloworld.h
	g++ -pthread helloworld-test.cpp ${USER_CXXFLAGS} -o helloworld-test
