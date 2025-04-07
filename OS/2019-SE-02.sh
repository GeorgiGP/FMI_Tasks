#!/bin/bash

set -e

if [[ ${#} -lt 2 ]]; then
    echo "Command ${0} should take 2 parameters!"
    exit 1
fi

var1=$1
var2=$2

if [[ ${1} -gt ${2} ]]; then
    var1=${2}
    var2=${1}
fi

join -t ':' -1 1 -2 1 -o 2.1 1.5 \
<(sort -t ':' -k 1,1 /etc/passwd) \
<(find /home/students -mindepth 1 -maxdepth 1 -printf "%C@ %p\n" | \
awk -F ' ' -v min=${var1} -v max=${var2} '( ( min < $1 ) && ( $1 < max ) ) { print $2 }' | \
cut -d '/' -f 4 | sort | uniq)
