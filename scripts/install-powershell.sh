#!/bin/bash

# This script abstracts away the necessary steps to install powershell on linux mint / ubuntu
# Source:
#    https://docs.microsoft.com/en-us/powershell/scripting/install/install-ubuntu


function ensure_root {
    local reason="$2"
    local root_userid=0;
    
    if [ $UID -ne $root_userid ]; then
        echo "$reason"
        exit 1
    fi
}

function ensure_installed {
    local command="$1"
    
    if ! which "$command"; then
        echo "This script requires the command '$command' to be installed, but it wasn't found."
        exit 2
    fi
}


ensure_root --reason '
    This script requires root privileges to:
       - Install package repositories
       - Update package sources
       - Install packages
'
ensure_installed apt-get
ensure_installed dpkg
ensure_installed wget


sudo apt-get update
sudo apt-get install -y wget apt-transport-https software-properties-common

# Download and register the Microsoft repository GPG keys
wget -q "https://packages.microsoft.com/config/ubuntu/$(lsb_release -rs)/packages-microsoft-prod.deb"
sudo dpkg -i packages-microsoft-prod.deb

sudo apt-get update
sudo apt-get install -y powershell
pwsh -v