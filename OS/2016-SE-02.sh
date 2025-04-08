#!/bin/bash

if [[ ${#} -lt 1 || ! $1 =~ ^[0-9]+$ ]]; then
    echo "Command should have 1 param number"
    exit 2
fi
if [[ ! $(whoami) == "s0600299" ]]; then
    echo "Script is run only from user root!"
    exit 3
fi

table=$(ps -e -o user,pid,rss | \
tail +2 | \
tr -s ' ' | \
sed -E "s/ *(.*)/\1/" |\
sort -t ' ' -k1,1 -k3,3rn)

curid=""
started=false

criteria=${1}
curSum=0
tempfile=$(mktemp)
while read row; do
    rowid=$(echo -e "${row}" | cut -d ' ' -f 1)
    if [[ ${curid} != ${rowid} ]]; then
        if [[ ! ${started} ]]; then
            started=true
        else
            echo -e "${curid} ${curSum}" >> tempfile
        fi

        if [[ ${curSum} -gt ${criteria} ]]; then
            kill $(echo -e "${row}" | cut -d ' ' -f 2)
        fi
        curid=${rowid}
        curSum=0
    fi
    curRSS=$(echo -e "${row}" | cut -d ' ' -f 3)
    curSum=$(( ${curRSS} + ${curSum} ))
done < <(echo -e "${table}")

cat tempfile
rm tempfile
