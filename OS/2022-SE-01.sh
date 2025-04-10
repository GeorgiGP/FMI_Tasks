#!/bin/bash

if [[ $# -lt 2 ]]; then
    echo "1 arg required!" 1>&2
    exit 2
fi

if [[ ! -f "$1" ]]; then
    echo "Config file "$1" does not exist." 1>&2
    exit 3
fi

if [[ ! -f "$2" ]]; then
    echo "Wakeup file "$2" does not exist." 1>&2
    exit 4
fi


configs=$(cat foo.conf |\
sed -E "s/^([^#]*)#.*$/\1/" |\
sed -E "s/[ \t]+/ /g" |\
sed -E "s/^ (.*)$/\1/" |\
egrep "^([A-Z0-9]+) (disabled|enabled)(| )$")

while read device status; do
    hasRow=$(cat "$2" | tail -n +2 | egrep "[ \t]*$device[ \t]+")

    if [[ -z "$hasRow" ]]; then
        echo "$device:device not found." 1>&2
        continue
    fi

    oldStatus=$(cat "$2" |\
    egrep "^([ \t]*$device[ \t]+[^ \t]+[ \t]+\*)(enabled|disabled)[ \t]" |\
    sed -E "s/^([ \t]*$device[ \t]+[^ \t]+[ \t]+\*)(enabled|disabled)[ \t].*$/\2/")

    if [[ "$oldStatus" == "$status" ]]; then
        continue
    fi
    if [[ "$oldStatus" == "disabled" ]]; then
        sed -E "s/^([ \t]*$device[ \t]+[^ \t]+[ \t]+\*)disabled/\1$status /" -i "$2"
    else
        sed -E "s/^([ \t]*$device[ \t]+[^ \t]+[ \t]+\*)enabled /\1$status/" -i "$2"
    fi
done < <(echo "$configs")
