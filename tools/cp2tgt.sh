#!/bin/bash
echo "Copying to target..."

sudo ifdown usb0
sudo ifup usb0

scp $1 root@10.9.8.2: