#!/bin/bash

if [[ $# -lt 1 ]]; then
    echo "At least 1 arg"
    exit 2
fi

if [[ ! -d $1 ]]; then
    echo "1st should be valid dir"
    exit 3
fi

hasNum=1
num=-1
if [[ ( $# -ge 2 ) && ( $2 =~ ^[1-9][0-9]* ) ]]; then
    hasNum=0
    num=${2}
fi

if [[ $hasNum -eq 0 ]]; then
    find $1 -links +$num
else
    find -L $1 -type l
fi
