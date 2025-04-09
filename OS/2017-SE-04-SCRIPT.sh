#!/bin/bash

if [[ $# -lt 1 ]]; then
    echo "At least 1 arg!"
    exit 2
fi

if [[ ! -d $1 ]]; then
    echo "Dir $1 does not exist"
    exit 3
fi

if [[ $# -lt 2 ||  ! -f $2 ]]; then #console
    find "$1" -type l -printf "%Y %p\n" | egrep -v "^N " | cut -d ' ' -f 2- | xargs -I{} find {} -printf "%p %l\n"
    echo "Dir $1 has: $(find "$1" -type l -printf "%Y\n" | egrep "^N$" | wc -l) broken symlinks!"
else #infile
    find "$1" -type l -printf "%Y %p\n" | egrep -v "^N " | cut -d ' ' -f 2- | xargs -I{} find "{}" -printf "%p %l\n" 1>"${2}"
    echo "Dir $1 has: $(find "$1" -type l -printf "%Y\n" | egrep "^N$" | wc -l) broken symlinks!" 1>> ${2}
fi
