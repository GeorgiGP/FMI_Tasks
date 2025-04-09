#!/bin/bash

set -u
if [[ $# -lt 1 ]]; then
    echo "1 arg!"
    exit 2
fi

if [[ ! -d $1 ]]; then
    echo "Arg must be dir"
    exit 3
fi

friends=$(find "$1" -mindepth 3 -maxdepth 3 | xargs -I{} basename "{}" | sort | uniq)

echo -e "All friends:\n$friends"

res=$(mktemp)
while read friend; do
    allfiles=$(find "$1" -mindepth 4 -maxdepth 4 -type f | egrep "/$friend/([0-9]){4}(-[0-9][0-9]){5}.t
    sumLines=0
    while read file; do
        sumLines=$(( $sumLines + $(cat $file | wc -l) ))
    done < <(echo "$allfiles")
    echo "$sumLines $(basename $friend)" >> "$res"
done < <(echo "$friends")

echo "Top 10 friends:"
cat "$res" | sort -t ' ' -k1,1rn | head -n 10

rm $res
