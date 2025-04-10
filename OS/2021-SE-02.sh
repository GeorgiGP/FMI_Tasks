#!/bin/bash

if [[ $# -lt 1 ]]; then
    echo "At least 1 file!" 1>&2
    exit 3
fi

for file in ${@}; do
    if [[ ! -f "$file" ]]; then
        echo "$file: Does not exist" 1>&2
        continue
    fi

    contains=$(cat "$file" | egrep "^.*(([0-9]){10}).*$" | sed -E "s/^.*(([0-9]){10}).*$/\1/")
    if [[ -z "$contains" ]]; then
        echo "$file: Does not have serial number" 1>&2
        continue
    fi

    id=$(echo $contains | sed -E "s/^(.*)([0-9][0-9])$/\2/")
    date=$(echo $contains | sed -E "s/^(.*)([0-9][0-9])$/\1/")

    today=$(date +"%Y%m%d")
    if [[ "$date" == "$(date +"%Y%m%d")" ]]; then
        id=$(echo $id | sed -E "s/^0([0-9])$/\1/")
        id=$(( $id + 1 ))
        if [[ $id -eq 100 ]]; then
            echo "$file: Serial date is 99 for today, can't have more changes for today" 1>&2
            continue
        fi
        id=$(echo $id | sed -E "s/^([1-9])$/0\1/")
    else
        date=$today
        id="00"
    fi

    sed -E "s/^(.*)$contains(.*)$/\1${date}${id}\2/" -i "$file"
done
