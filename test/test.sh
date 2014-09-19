#!/bin/bash -ue

if [ -x /sys/module/helloworld_test ] ; then
    rmmod helloworld_test
fi

if [ -x /sys/module/helloworld_loopback ] ; then
    rmmod helloworld_loopback
fi

if [ -x /sys/module/helloworld ] ; then
    rmmod helloworld
fi

cd `dirname ${BASH_SOURCE[0]}`

modparams=

for i in "$@" ; do
    case $i in
        debug)
            modparams="${modparams} dyndbg='=pflt'"
	    ;;
    esac
done

insmod ../helloworld.ko ${modparams}
insmod ../helloworld-loopback.ko ${modparams}
./helloworld-test
