#!/bin/bash

if [ ! -e "../container_root" ]
then
    echo "Unable to find ../container_root; run ../set_up_root_filesystem.sh"
    exit 1
fi

cp bin/* ../container_root/usr/local/bin
