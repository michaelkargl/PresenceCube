#!/bin/bash

ROOT_USERID=0
STATUS_CODE__NOT_FOUND=2
STATUS_CODE__NOT_PERMITTED=3

# This script abstracts away the necessary steps to install powershell on linux mint / ubuntu
# Source:
#    https://docs.microsoft.com/en-us/powershell/scripting/install/install-ubuntu

# @synopsis ensures the current session is running in elevated mode
# @parameter reason: the reason for requiring elevated access.
# @example ensure_root --reason 'Requires root privileges to install missing packages'
# @status 0: OK
# @status 1: Session not in privileged mode
function ensure_root {
    local reason="$2"

    if [ $UID -ne $ROOT_USERID ]; then
        echo "$reason"
        exit $STATUS_CODE__NOT_PERMITTED
    fi
}

function ensure_installed {
    local command="$1"
    
    if ! which "$command"; then
        echo "This script requires the command '$command' to be installed, but it wasn't found."
        exit $STATUS_CODE__NOT_FOUND
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
