#!/bin/bash

alg=SGA
options="-alg $alg -iter 10000 -pop 30 -cross 1.0"
inits=("Random" "CNN" "ClosestNE" "FarthestNE" "NEHS")

for file in `ls data/*.csv`
do
	l=`expr ${#file} - 9`
	f=${file:5:$l}
	for init in ${inits[@]}
	do
		./fold.sh test/$alg-$f-$init.csv $options -f $file -init $init
	done
done