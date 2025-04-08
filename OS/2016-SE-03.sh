#!/bin/bash

set -e -u

if [[ ! $(whoami) == "s0600299" ]]; then
    echo "Script can only be executed by root"
    exit 2
fi

all_users_dirs=$(cat /etc/passwd | cut -d ':' -f 1,3,4,6)

while read curRow; do
    curDir=$(echo "${curRow}" | cut -d ':' -f 4)
    curUser=$(echo "${curRow}" | cut -d ':' -f 1)

    if [[ ! -d "${curDir}" ]]; then
        echo -e "${curUser}"
        continue;
    fi

    userId=$(id -u "${curUser}")
    groupId=$(id -g "${curUser}")

    fileUserId=$(stat "${curDir}" -c "%u")
    fileGroupId=$(stat "${curDir}" -c "%g")

    if [[ ("${userId}" == "${fileUserId}") && ( $(( $(stat "${curDir}" -c "%a") & 0200 )) -eq 0200 ) ]]
        :
    elif [[ ("${groupId}" -eq "${fileGroupId}") && ( $(( $(stat "${curDir}" -c "%a") & 0020 )) -eq 0020
        :
    elif [[ $(( $(stat "${curDir}" -c "%a") & 0002 )) -eq 0002 ]]; then
        :
    else
        echo "${curUser}"
    fi
done < <(echo "${all_users_dirs}")
