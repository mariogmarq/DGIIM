#!/bin/bash
if [[ $# != 1 ]]; then
	exit 1
fi

grep -v "^[#,$]" $1 > NuevoArchivo
mv NuevoArchivo $1