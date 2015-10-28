#!/bin/bash
echo "Connecting to target..."

sudo ifdown usb0
sudo ifup usb0

ssh root@10.9.8.2