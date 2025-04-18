#!/bin/bash

if [[ $# -lt 2 ]]; then
    echo "2 params!"
    exit 3
fi

if [[ -f "$1" ]]; then
    echo "1st param existing file"
    exit 4
fi

if [[ ! -d "$2" ]]; then
    echo "2nd param non existing dir"
    exit 5
fi

allLogs=$(find "$2" -type f | egrep "\.log$")

function extract {
    egrep "$1 *: *([^ ].*)$" | sed -E "s/^.*$1 *: *([^ ]+) *$/\1/"
}

echo "hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key" > "$1"
while read log; do
    infoCur=$(cat "$log")
    echo -n "$(basename "$log" | sed -E "s/(.*)\.log/\1/")," >> "$1"
    echo -n "$(echo "$infoCur" | extract "Maximum Physical Interfaces")," >> "$1"
    echo -n "$(echo "$infoCur" | extract "VLANs")," >> "$1"
    echo -n "$(echo "$infoCur" | extract "Inside Hosts")," >> "$1"
    echo -n "$(echo "$infoCur" | extract "Failover")," >> "$1"
    echo -n "$(echo "$infoCur" | extract "VPN-3DES-AES")," >> "$1"
    echo -n "$(echo "$infoCur" | extract "\*Total VPN Peers")," >> "$1"
    echo -n "$(echo "$infoCur" | extract "VLAN Trunk Ports")," >> "$1"
    echo -n "$(echo "$infoCur" |  egrep "This platform has (a|an) (.*) license." | sed -E "s/This platf
    echo -n "$(echo "$infoCur" | extract "Serial Number")," >> "$1"
    echo "$(echo "$infoCur" | extract "Running Activation Key")" >> "$1"
done < <(echo "$allLogs")
