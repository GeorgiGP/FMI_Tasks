#!/bin/bash

num=10
if [[ "$1" == "-n" ]]; then
    if [[ ! $2 =~ ^[0-9]+$ ]]; then
        echo "Invalid num!"
        exit 3
    fi
    num=$2
    shift 2
fi

tempfile=$(mktemp)
for file in ${@}; do
    name=$(echo "$file" | sed -E "s/^(.*)\.log$/\1/")
    cat "$file" |\
    tail -n $num |\
    sed -E "s/^(([0-9]){4}(-[0-9][0-9]){2}) ([0-2][0-9]:[0-5][0-9]:[0-5][0-9]) ([^ ].*)$/\1 \4 $name \5
done

cat "$tempfile" | sort -t ' ' -k1,1r -k2,2r

rm $tempfile
