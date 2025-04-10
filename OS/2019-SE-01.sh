#!/bin/bash


tempfile=$(mktemp)
tempfile2=$(mktemp)
while read row; do
    if [[ -z $(echo "$row" | egrep "(^ *-[1-9][0-9]* *$)|(^ *[1-9][0-9]* *$)") ]]; then
        continue
    fi
    echo "$row" | tr -d ' ' >> "$tempfile"
    echo "$row" | tr -d ' ' | awk -F ' ' '$1 < 0 { printf("%s\n",-$1) } $1 >= 0 { printf("%s\n",$1) }'
done

lam=$(cat "$tempfile" | sort -n | uniq)
echo "$lam" 1>"$tempfile"
biggestAbsNum=$(cat "$tempfile2" | sort -t ' ' -k1,1nr | head -n 1)


echo "a) Biggest abs nums:"
while read num; do
    if [[ ( $num -eq $biggestAbsNum ) || ( $(( -1 * $num )) -eq $biggestAbsNum ) ]]; then
        echo "$num"
    fi
done < <(cat "$tempfile")

function digitSum {
    num=$1
    if [[ $(echo "$num < 0" | bc -l) -eq 1 ]]; then
        num=$(( -1 * $num ))
    fi
    echo "$(( $(echo $num | sed -E "s/(.)/\1+/g" | sed -E "s/^(.*)\+$/\1/") ))"
}

while read num; do
    sumD=$(digitSum $num)
    echo "$num $sumD"
done < <(cat "$tempfile") > "$tempfile2"

maxSum=$(cat "$tempfile2" | sort -t ' ' -k2,2nr -k1,1n | head -n 1 | cut -d ' ' -f 2)
echo "b) Biggest digit sum:"
cat "$tempfile2" | egrep " $maxSum" | sort -t ' ' -k1,1n | head -n 1 | cut -d ' ' -f 1


rm $tempfile
rm $tempfile2
