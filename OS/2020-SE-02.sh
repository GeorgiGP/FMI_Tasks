#!/bin/bash

set -e -u

table=$(cat ${1})

launch=$(echo -e "${table}" | egrep "^([^\|]*\|){2}Failure\|" | \
cut -d '|' -f 2 | \
sort | \
uniq -c | \
sed -E "s/^ *([^ ].*)/\1/" | \
sort -t ' ' -k1,1rn | \
head -n 1 | \
cut -d ' ' -f 2)

echo -e "${table}" | egrep "^([^\|]*\|)${launch}\|" | sort -t '|' -k1,1r | head -n 1 | cut -d '|' -f 3,4 | tr '|' ':'
