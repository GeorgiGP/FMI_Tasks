#!/bin/bash

if [[ $# -lt 2 ]]; then
    echo "2 args!"
    exit 2
fi

if [[ ! -f "$1" || ! -d "$2" ]]; then
    echo "1st file and 2nd dir!"
    exit 3
fi

if [[ ! -z $(find "$2" -mindepth 1) ]]; then
    echo "Dir should be empty!"
    exit 4
fi

fileinfo=$(cat "${1}" |\
sed -E "s/^ *([a-zA-Z-]+) +([a-zA-Z-]+) *([^ ].*)$/\1 \2 \3/" |\
sed -E "s/^([a-zA-Z-]+) ([a-zA-Z-]+)([^a-zA-Z -].*)/\1 \2 \3/")


users=$(echo "${fileinfo}" |\
cut -d ' ' -f 1,2 |\
sort | uniq |\
awk -F ' ' '{ printf("%s %s;%s\n",$1,$2,NR) }')

touch "$2/dict.txt"
echo "$users" > "$2/dict.txt"

users=$(echo "$users" | tr ';' ' ')
echo "$users"
while read name last num; do
    touch "$2/${num}.txt"
    echo "$fileinfo" | egrep "^$name $last " > "$2/$num.txt"
done < <(echo "$users")
