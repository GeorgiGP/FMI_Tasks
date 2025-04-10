#!/bin/bash

if [[ $# -lt 2 ]]; then
    echo "2 args!"
    exit 2
fi

if [[ ! -f "$1" || ! -f "$2" ]]; then
    echo "Both should be valid file names!"
    exit 3
fi

uniqRows=$(cat "$1" | cut -d ',' -f 2- | sort | uniq)

while read row; do
    cat "$1" | egrep "[0-9]+,${row}" | sort -t ',' -k1,1n | head -n 1 >> "$2"
done < <(echo "$uniqRows")
