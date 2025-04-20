#!/bin/bash

table=$(ps -e -o user,uid,pid,rss | tail -n +2 | tr -s ' ' | sed -E "s/ *([^ ].*)/\1/")

while read uid; do
    sumRss=$(echo "$table" |\
    egrep "^[^ ]* $(echo $uid | cut -d ' ' -f 2)" |\
    awk -F ' ' -v cnt=0 '{ cnt = cnt + $4 } END { printf("%s",cnt) }')
    echo -e "${uid} ${sumRss}"
done < <(echo "${table}" | cut -d ' ' -f 1,2 | sort -t ' ' -k1,1 -k2,2 | uniq)

while read uid; do
    id=$(echo $uid | cut -d ' ' -f 2)
    avgRss=$(echo "$table" |\
    egrep "^[^ ]* ${id}" |\
    awk -F ' ' -v cnt=0 '{ cnt = cnt + $4 } END { printf("%s",cnt / NR) }')
    biggestProcessRow=$(echo "$table" | egrep "^[^ ]* ${id} " | sort -t ' ' -k4,4rn | head -n 1)

    if [[ $(echo "$(echo "$biggestProcessRow" | cut -d ' ' -f 4) > 2 * ${avgRss}" | bc -l) -eq 1 ]]; then
        kill $(echo "${biggestProcessRow}" | cut -d ' ' -f 3)
    fi
done < <(echo "${table}" | cut -d ' ' -f 1,2 | sort -t ' ' -k1,1 -k2,2 | uniq)
