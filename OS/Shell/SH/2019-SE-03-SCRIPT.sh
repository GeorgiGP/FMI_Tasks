#!/bin/bash

if [[ $# -lt 1 ]]; then
    echo "1 erg dir!"
    exit 3
fi

if [[ ! -d $1 ]]; then
    echo "First arg must be a dir"
    exit 2
fi

tempfile=$(mktemp)
allTGZ=$(find "$1" | egrep "^[^_]*_report-[1-9][0-9]*.tgz$" | xargs -I{} realpath "{}" )

while read tgz; do
    sha256sum "${tgz}" | tr -s ' ' >> "$tempfile"
done < <(echo "${allTGZ}")

if [[ ! -f "checksums.txt" ]]; then
    touch "checksums.txt"
fi

allChecks=$(join -t ' ' -1 2 -2 2 -a 1 -o 1.1,2.1,1.2 "$tempfile" "checksums.txt")

while read rowChecks; do
    ch1=$(echo "$rowChecks" | cut -d ' ' -f 1)
    ch2=$(echo "$rowChecks" | cut -d ' ' -f 2)
    path=$(echo "$rowChecks" | cut -d ' ' -f 3)
    if [[ "$ch1" == "$ch2" ]]; then
        continue
    fi

    tempdir=$(mktemp -d)
    tar -x -f $path --one-top-level=$tempdir

    while read file; do
        if [[ -z $file ]]; then
            continue
        fi
        mkdir -p "extracted"

        filename=$(echo "$path" | sed -E "s:.*/(.*_)report-(.*).tgz$:\1\2.txt:")
        mv "$file" "extracted/$filename"
        break
    done < <(find $tempdir -type f | egrep "meow.txt$")

    rm -r $tempdir
done < <(echo "$allChecks")

echo "$allChecks" | cut -d ' ' -f 1,3 > "checksums.txt"

rm $tempfile
