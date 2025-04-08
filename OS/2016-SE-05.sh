#!/bin/bash

set -e -u

if [[ $# -lt 2 ]]; then
    echo "2 args!"
    exit 3
fi

if [[ ! -f $1 || ! -f $2 ]]; then
    echo "Files should exist!"
    exit 2
fi

function g {
    cat $1 | egrep "$1" | wc -l
}

fWin=
if [[ $(g "$1") -gt $(g "$2") ]]; then
    fWin=$1
else
    fWin=$2
fi

cat $fWin | sed -E "s/^[^-]- *([^ ].*)/\1/" | sort > "${fWin}.songs"
