#!/bin/bash

case "$1" in
    load)
        t=`dirname ${BASH_SOURCE}`
        insmod ${t}/../helloworld.ko || exit -1
        ;;
    unload)
        if [ -d /sys/module/helloworld ] ; then
            rmmod helloworld || exit -1
        fi
        ;;
    *)
    echo Invalid command $1 2>&1
    exit -1
    ;;
esac

