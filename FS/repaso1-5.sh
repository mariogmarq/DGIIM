#!/bin/bash

# 1. mkdir curso_1920 cuatrimestre_1 cuatrimestre_2
# 2. touch curso_1920/repasoMod1 && (printf "DNI: \nNombre y apellidos: \nFecha: " > curso_1920/repasoMod1)
# 3. cp curso_1920/repasoMod1 cuatrimestre_1
# 4. date >> curso_1920/repasoMod1
# 5. RUTA=`pwd`	set
# 6. man find | tail -n 20 >> cuatrimestre_2/ayudafind


if [[ $# -ne 3 ]]; then
	echo "Only do 3 arguments";
	exit 1;
fi
if [[ $2 != ['f','d'] ]]; then
	echo "Not good argument $2";
	exit 1;
fi

if [[ !(-e $1) ]]; then
	mkdir $1;
fi

(grep -Rn $3 $1) > resultado;

if [[ $2 == 'f' ]]; then
	( ls -p $1 | grep -v /) >> resultado;
else
	( ls -p $1 | grep '/') >> resultado;
fi
