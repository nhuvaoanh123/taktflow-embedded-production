#!/bin/bash
# Setup SSH key auth for desktop PC access
# Usage:
#   Laptop (Ubuntu):  bash setup_ssh.sh laptop
#   Pi (QNX):         bash setup_ssh.sh qnx

DESKTOP_KEY="ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIIX5UK2qhzR+NsNDASqWImsyDHHlrdRE0ta6fpHosbNB andao@DESKTOP-0TAVV7M"
LAPTOP_KEY="ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIEE5Wd7IM9aZyuqIpUxkMn+DLu/UoCnQOHhS0Kz8zDKw andao-laptop-to-pi"

TARGET=${1:-laptop}

if [ "$TARGET" = "qnx" ]; then
    mkdir -p /root/.ssh
    echo "$DESKTOP_KEY" > /root/.ssh/authorized_keys
    echo "$LAPTOP_KEY" >> /root/.ssh/authorized_keys
    chmod 700 /root/.ssh
    chmod 600 /root/.ssh/authorized_keys
    cat /root/.ssh/authorized_keys
    echo "DONE — SSH keys installed for QNX root"
elif [ "$TARGET" = "laptop" ]; then
    mkdir -p /home/an-dao/.ssh
    echo "$DESKTOP_KEY" > /home/an-dao/.ssh/authorized_keys
    chown an-dao:an-dao /home/an-dao/.ssh
    chown an-dao:an-dao /home/an-dao/.ssh/authorized_keys
    chmod 700 /home/an-dao/.ssh
    chmod 600 /home/an-dao/.ssh/authorized_keys
    cat /home/an-dao/.ssh/authorized_keys
    echo "DONE — SSH keys installed for laptop"
else
    echo "Usage: bash setup_ssh.sh [laptop|qnx]"
    exit 1
fi
