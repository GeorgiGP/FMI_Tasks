#!/bin/bash

if [[ ${#} -lt 1 ]]; then
    echo "Command takes 1 argument directory path"
    exit 2
fi

if [[ ! -d ${1} ]]; then
    echo "Argument must be directory"
    exit 1
else
    find ${1} -type l -printf "%p\0%Y\n" | awk -F '\0' '$2 == "N" { print $1 }'
fi
