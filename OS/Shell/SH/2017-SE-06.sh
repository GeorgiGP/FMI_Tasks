#!/bin/bash

if [[ ${USER} != "s0600299" ]]; then
    echo "Only from root user can be executed"
    exit 2
fi

table=$(ps -e -o user=,uid=,pid=,rss= |\
tr -s ' ' |\
sed -E "s/^ *([^ ].*)/\1/" |\
sort -t ' ' -k2,2n)

function getSumRssFromId {
    echo "$table" |\
    egrep "^[^ ]+ $1 " |\
    awk -F ' ' -v cnt=0 '{ cnt = cnt + $4 } END { printf("%s", cnt) }'
}

sumRootRss=$(getSumRssFromId 0)

while read user uid ; do
    echo "$user : $(getSumRssFromId $uid)"

    curDir="$(cat /etc/passwd | egrep "^([^:]+:){2}$uid:" | cut -d ':' -f 6)"
    if [[ ! -d "$curDir" ]]; then
        :
    elif [[ ! $(stat ${curDir} -c "%u") -eq $uid ]]; then
        :
    elif [[ $(( 0$(stat ${curDir} -c "%a") & 0400 )) -eq 0 ]]; then
        :
    else
        continue
    fi
    echo "$user : $(getSumRssFromId $uid) IS CHECKED TO BE KILLED"
    if [[ $(getSumRssFromId $uid) -gt $sumRootRss ]]; then
        echo "$user -> killed"
        killall -u $user
    fi
done < <(echo "$table" | cut -d ' ' -f 1,2 | uniq | egrep -v "^root ")
