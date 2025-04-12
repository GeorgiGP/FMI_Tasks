#!/bin/bash

set -e

if [[ $# -lt 2 ]]; then
    echo "2 args!" >&2
    exit 3
fi

seconds=$1

shift 1

now=$(date +%s.%N)
until=$(echo "$now + $seconds" | bc -l)

cnt=0

while [[ $(echo "$(date +%s.%N) < $until" | bc -l) -eq 1 ]]; do
    $@
    cnt=$(( $cnt + 1 ))
done

after=$(date +%s.%N)

time=$(echo "$after - $now" | bc -l)
roundedTime=$(echo "$time" | awk '{ printf("%.2f", $1) }')
echo "Time: $roundedTime"
echo "Script called: $cnt"

avg=$(echo "$time/$cnt" | bc -l | awk '{ printf("%.2f", $1) }')
echo "Avg per call: $avg"
