#!/bin/bash

if [[ $# -lt 1 ]]; then
    echo "1 arg!"
    exit 2
fi

if [[ ! -f $1 ]]; then
    echo "Should be normal file"
    exit 3
fi

cat $1 | sed -E "s/^[^-]*- *([^ ].*)/\1/" | awk -F ' ' '{ printf("%s. %s\n",NR,$0) }' | sort -t ' ' -k2
