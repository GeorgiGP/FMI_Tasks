#!/bin/bash

if [[ $# -lt 3 ]]; then
    echo "3 args!"
    exit 3
fi

if [[ ! -f ${1} ]]; then
    echo "First arg should be valid file!"
    exit 2
fi
rowKey2=$(cat "${1}" | grep -E "^ *${3} *=(.*)")
if [[ -z "${rowKey2}" ]]; then
    echo "No key2."
    exit 0
fi

rowKey1=$(cat "${1}" | grep -E "^ *${2} *=(.*)")

valuesRow1=$(echo ${rowKey1} | sed -E "s/^.*=(.*)/\1/" | tr -d ' ')
valuesRow2=$(echo ${rowKey2} | sed -E "s/^.*=(.*)/\1/" | tr ' ' '\n')

res=""
while read ch; do
    if [[ -z $(echo "$valuesRow1" | egrep "${ch}") ]]; then
        res="${ch} ${res}"
    fi
done < <(echo "${valuesRow2}")

sed -E "s/^ *$3 *=(.*)/$3=${res}/"  -i "${1}"
