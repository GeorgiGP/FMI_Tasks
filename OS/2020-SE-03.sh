#!/bin/bash

if [[ $# -lt 2 ]]; then
    echo "2 args!"
    exit 2
fi

if [[ ! -d "$1" || ! -d "$2" ]]; then
    echo "2 should be valid dirs"
    exit 3
fi

repoName=$(realpath "$1")
packageName=$(realpath "$2")

tempdir=$(mktemp -d)
toSearchForName=$(basename $(echo "$packageName-$(cat "$packageName/version")"))

tar -c -f "$tempdir/temp" "$packageName/tree/" 2>/dev/null
xz "$tempdir/temp"
checksum=$(sha256sum "$tempdir/temp.xz" | cut -d ' ' -f 1)

row=$(cat "$repoName/db" | egrep "$toSearchForName")
if [[ -z ${row} ]]; then
    echo "$toSearchForName $checksum" >> "$repoName/db"
    lambda=$(sort "$repoName/db")
    echo "$lambda" > "$repoName/db"
else
    oldChecksum=$(echo "$row" | cut -d ' ' -f 2)
    sed -E "s/^${row}$/$toSearchForName $checksum/" -i "$repoName/db"
    rm "$repoName/packages/$oldChecksum.tar.xz"
fi

mv "$tempdir/temp.xz" "$repoName/packages/$checksum.tar.xz"

rm -r "$tempdir"
