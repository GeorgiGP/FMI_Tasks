#!/bin/bash

inode=$(find /home/velin -printf "%C@ %i\n" 2>/dev/null | sort -t ' ' -k1,1rn | head -n 1 | cut -d " " -f 2)

find /home/velin -inum ${inode} -printf "%d %p\n" 2>/dev/null | sort -t ' ' -k1,1 | head -n 1 | cut -d ' ' -f 1
