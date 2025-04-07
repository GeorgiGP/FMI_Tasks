#!/bin/bash

cat example-passwd | \
grep -E --color '^[^:]*:[^:]*:[^:]*:[^:]*:[a-zA-Z ]*a[,: ]' | \
egrep --color '[Ii][Nn][Ff]' | \
egrep --color '^[^:]*[0-9]{3,}' | \
cut -d : -f 1 | \
sed -E "s/^[^0-9]*([0-9]+)[^0-9]*/\1/" | \
cut -c 2,3 | \
sort -n | \
uniq -c | \
sort -n -r | \
head -n 1
