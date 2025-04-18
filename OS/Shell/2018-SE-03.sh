#!/bin/bash

if [[ ! ${#} -eq 1 ]]; then
    echo "Command ${0} should take 1 arg"
    exit 1
fi

if [[ ! -f ${1} ]]; then
    echo "File ${1} does not exist"
    exit 2
fi

cat ${1} | \
egrep "^([^:]*:){5} */home/SI/.*" | \
cut -d ':' -f 5,6 | \
sed -E "s/^(([a-zA-Z]+ )*[a-zA-Z]+) *[^a-zA-Z: ].*: *(.*)/\1:\3/" | \
sed -E "s/^(([a-zA-Z]+ )*[a-zA-Z]+) *: *(.*)/\1:\3/"
