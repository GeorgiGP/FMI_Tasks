#!/bin/bash

set -u -e
userRes=
fileRes=
greatestTime=-1

tempfile=$(mktemp)
while read pair; do
    user=$(echo "$pair" | cut -d ':' -f 1)
    dir=$(echo "$pair" | cut -d ':' -f 2)

    curDirLastChanged=$(find "${dir}" -type f -printf "%C@ %p\n" 2>/dev/null | sort -t ' ' -k1rn | head

    if [[ -z "${curDirLastChanged}" ]]; then
        continue
    fi

    curTime=$(echo "${curDirLastChanged}" | cut -d ' ' -f 1)
    curPath=$(echo "${curDirLastChanged}" | cut -d ' ' -f 2-)
    echo "${curDirLastChanged}" >> $tempfile

    if [[ "$(echo "${curTime} > ${greatestTime}" | bc -l)" -eq 1 ]]; then
        greatestTime=$curTime
        userRes="$user"
        fileRes="$curPath"
    fi

done < <(cat /etc/passwd | cut -d ':' -f 1,6)

echo "${userRes} -> ${fileRes}"
cat $tempfile | sort -t ' ' -k1,1rn | head -n 5
rm $tempfile
