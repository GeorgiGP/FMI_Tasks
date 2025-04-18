#!/bin/bash

find . -printf '%n\t%p\n' | sort -t $'\t' -k1,1r -k2 | cut -d $'\t' -f 2- | head -n 5
