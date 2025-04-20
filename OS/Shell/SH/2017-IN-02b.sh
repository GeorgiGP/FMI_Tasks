#!/bin/bash

while read file; do
    rm "${file}"
done < <(find ~ -printf "%s\0%p\n" | sort -n -r | head -n 5 | cut -d $'\0' -f 2)
