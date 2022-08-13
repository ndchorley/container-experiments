#!/bin/bash

if [ ! -e "bin" ]
then
    mkdir bin
fi

gcc -static -o bin/allocate_memory allocate_memory.c
