#!/bin/bash
SOL1=`bc <<< "( -$2 + sqrt($2^2 - 4*$1*$3) ) / 2*$1"`
SOL2=`bc <<< "( -$2 - sqrt($2^2 - 4*$1*$3) ) / 2*$1"`
echo -e "Sol1: $SOL1\n Sol2: $SOL2"
