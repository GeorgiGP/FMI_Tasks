#!/bin/bash

if [[ $# -lt 2 ]]; then
    echo "At least 2 params"
    exit 2
fi

if [[ ! -d $1 ]]; then
    echo "1st param is not a valid dir!"
    exit 3
fi

allMatches=$(find "$1" -maxdepth 1 |\
egrep "/vmlinuz-[0-9]+\.[0-9]+\.[0-9]+-${2}$")

winnerVersion=$(echo "$allMatches" |\
sed -E "s:(.*)/vmlinuz-(.*):\2:" |\
sed -E "s/(.*)-${2}$/\1/" |\
cut -d "." -f 1,2,3 |\
sort -t '.' -k1,1rn -k2,2rn -k3,3rn |\
head -n 1)

num1=$(echo "$winnerVersion" | cut -d '.' -f 1)
num2=$(echo "$winnerVersion" | cut -d '.' -f 2)
num3=$(echo "$winnerVersion" | cut -d '.' -f 3)
echo "$allMatches" | egrep "${num1}\.${num2}\.${num3}"
