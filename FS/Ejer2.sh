#!/bin/bash
if [[ -z lsusb ]]; then
    echo "No hay dispositivos conectados"
else
    lsusb
fi
