#!/bin/bash

alpine_filename="alpine-minirootfs-3.16.1-x86_64.tar.gz"

download_alpine () {
    if [ ! -e $alpine_filename ]
    then
        echo "Downloading Alpine"
        curl \
            --output $alpine_filename \
            "https://dl-cdn.alpinelinux.org/alpine/v3.16/releases/x86_64/$alpine_filename"
    else
        echo "Found Alpine, not downloading"
    fi

    echo
}

create_container_root_filesystem () {
    download_alpine

    directory=$1

    echo "Creating container root filesystem in $directory"

    rm -rf $directory && mkdir $directory
    tar -C $directory -xf $alpine_filename

    touch $directory/CONTAINER_ROOT

    echo
}

create_container_root_filesystem "container_root"
