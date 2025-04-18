#!/bin/bash

set -e -u

home_dir=$(cat /etc/passwd | egrep "^$(whoami):.*" | cut -d ':' -f 6)

find ${home_dir} -perm 644 -exec chmod 664 {} \;
