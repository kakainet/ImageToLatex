#!/bin/bash

if [ $# != 2 ];
then
	echo "required args: program name and path";
	exit;
fi

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'
EXPRESSIONS_NUMBER=$1
BATCH_SIZE=$2
let "STEPS = $1/$2"
echo $STEPS
for (( i=1; i<=$STEPS; i++ ))
do  
   echo "./l1_1.native $EXPRESSIONS_NUMBER > out/raw_tex${i}.in"
   ./l1_1.native $EXPRESSIONS_NUMBER > "out/raw_tex${i}.in"
done
