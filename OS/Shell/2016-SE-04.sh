#!/bin/bash

if [[ ${#} -lt 3 || ! "$1" =~ ^[0-9]*$ || ! "$2" =~ ^[0-9]*$ ]]; then
    echo "3 params needed - 2nums and 1 dir to move from"
    exit 2
fi

if [[ ! -d "$3" ]]; then
    echo "Not a dir"
    exit 3
fi

if [[ ! -d "a" ]]; then
    mkdir "a"
fi

if [[ ! -d "b" ]]; then
    mkdir "b"
fi

if [[ ! -d "c" ]]; then
    mkdir "c"
fi

while read file; do
    if [[ ! -f $file ]]; then
        continue
    fi
    count=$(cat $file | wc -l)
    base=$(basename $(realpath ${file}) )
    if [[ count -lt $1 ]]; then
        mv ${file} a/${base}
    elif [[ count -lt $2 ]]; then
        mv ${file} b/${base}
    else
        mv ${file} c/${base}
    fi
done < <(find $3)
