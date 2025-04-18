#!/bin/bash

if [[ $# -lt 1 ]]; then
    echo "1 arg!"
    exit 2
fi

if [[ -z $(cat /etc/passwd | cut -d ':' -f 1 | egrep "$1") ]]; then
    echo "This user does not exist!"
    exit 3
fi

table=$(ps -e -o user,uid,pid,time |\
tail -n +2 |\
tr -s ' ' |\
sed -E "s/^ *([^ ].*)/\1/" |\
sort -t ' ' -k1,1)

function get_pr_count_for_user {
    pr_target_has=$(echo "$table" |\
    cut -d ' ' -f 1 |\
    uniq -c |\
    egrep "$1")

    count_pr_target=

    if [[ -z ${pr_target_has} ]]; then
        count_pr_target=0
    else
        count_pr_target=$(echo "$pr_target_has" |\
        tr -s ' ' |\
        sed -E "s/^ *([^ ].*)/\1/" |\
        cut -d ' ' -f 1)
    fi

    echo -e "${count_pr_target}"
}

main_user_pr_count=$(get_pr_count_for_user $1)

while read user; do
    if [[ $(get_pr_count_for_user ${user}) -gt ${main_user_pr_count} ]]; then
        echo -e "${user}"
    fi
done < <(echo "$table" | cut -d ' ' -f 1 | uniq)

#b)
avg_time=$(echo "${table}" |\
cut -d ' ' -f 4 |\
awk -F ':' -v cnt=0 \
'{ cnt = cnt + $1 * 60 * 60 + $2 * 60 + $3 }
END { printf("%s\n", cnt / NR) }')

#c)

function get_time {
    awk -F ':' '{ print ($1 * 60 * 60 + $2 * 60 + $3) }'
}

twice_avg=$(echo "2 * ${avg_time}" | bc)
while read process; do
    curTime=$(echo "$process" | cut -d ' ' -f 4 | get_time)
    if [[ $(echo "${curTime} > ${twice_avg}" | bc -l) ]]; then
        kill $(echo "${process}" | cut -d ' ' -f 3)
    fi
done < <(echo "${table}" | egrep "^${1} ")
