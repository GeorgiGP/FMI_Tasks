#!/bin/bash

find ~ -mindepth 1 -maxdepth 1 -type f -user "$(whoami)" -exec chmod 664 {} &>/dev/null \;
