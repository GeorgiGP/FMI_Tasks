#!/bin/bash

if [[ $# -lt 2 ]]; then
    echo "2 args!" 1>&2
    exit 3
fi

if [[ ! -f "$1" ]]; then
    echo "1st must be a valid file" 1>&2
    exit 4
fi

if [[ ! -d "$2" ]]; then
    echo "2nd must be a valid dir" 1>&2
    exit 5
fi

allFiles=$(find "$2" -type f | egrep ".txt$")

while read word; do
    count=$(( $(echo "$word" | wc -m) - 1))
    cens=$(cnt=0
    max=$count
    while [[ cnt -lt $max ]]; do
        echo -n "*"
        cnt=$(( $cnt + 1))
    done)
    while read file; do
        sed -E "s/([^a-z0-9_])$word([^a-z0-9_])/\1$cens\2/gi" -i "$file"|\
        sed -E "s/([^a-z0-9_])$word$/\1$cens/gi" -i "$file"|\
        sed -E "s/^$word([^a-z0-9_])/$cens\1/gi" -i "$file"|\
        sed -E "s/^$word$/$cens/gi" -i "$file"
    done < <(echo "$allFiles")
done < <(cat "$1")
