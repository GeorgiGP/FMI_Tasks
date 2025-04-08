#!/bin/bash

if [[ $(whoami) != "s0600299" ]]; then
    echo "Only from root!"
    exit 4
fi

if [[ $# -lt 3 ]]; then
    echo "3 params"
    exit 3
fi

if [[ ! -d $1 || ! -d $2 ]]; then
    echo "Dirs should be valid as 1st 2 parameters"
    exit 2
fi

if [[ $(find ${1} -mindepth 1 | wc -l) -gt 0 ]]; then
    echo "1st dir is srcdir. Should be emptry!"
    exit 5
fi

realpSrc=$(realpath ${1})
realpDest=$(realpath ${2})
while read file; do
    toCreate=$(echo -e "${file}" | sed -E "s:^${realpSrc}(/.*):${2}\1:")
    mkdir -p $(dirname ${toCreate})
    mv "${file}" "${toCreate}"
done < <(find ${realpSrc} -mindepth 1 | egrep "${3}")
