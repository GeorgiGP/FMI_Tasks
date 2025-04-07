#!/bin/bash

set -e

if [[ ! ${#} -eq 1 ]]; then
    echo "Command ${0} should take only 1 argument"
    exit 1
fi

full_name=$(cat /etc/passwd | egrep "^${1}" | cut -d ':' -f 6)

find ${full_name} -links +1 -printf "%n:%i:%TY:%Tj:%TH:%TM:%TS:%p\n" 2>/dev/null | \
sort -t ':' -k3r | \
head -n 1 | \
cut -d ':' -f 2
