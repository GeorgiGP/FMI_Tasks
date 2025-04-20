#!/bin/bash

if [[ ${#} -ne 1 ]]; then
    echo "Command ${0} should have 1 argument"
    exit 1
fi

if [[ ! -f ${1} ]]; then
    echo "File ${1} does not exist"
    exit 2
fi

grep -E "^[^a-w]*[02468][^a-w]*$" ${1} | wc -l
