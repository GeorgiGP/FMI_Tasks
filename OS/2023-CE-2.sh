#!/bin/bash

set -e -u

result=0
while read -d $'\n' file; do
    result=$(( $(egrep -o "error" ${file} | wc -l)  + ${result} ))
done < <(find / -type f 2>/dev/null | egrep "/[a-zA-Z0-9_]+_[1-9][0-9]*\.log$")

echo "${result}"
