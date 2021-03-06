#!/bin/bash

alg=PBIL
options="-alg $alg -iter 1000 -pop 40 -lr 0.1 -nlr 0.01 -bit 0.25"
inits=("Random" "CNN" "ClosestNE" "FarthestNE" "NEHS")

./fold.sh test/$alg-$1-Unif25.csv $options -f data/$1.csv

#for file in `ls data/*.csv`
#do
#	l=`expr ${#file} - 9`
#	f=${file:5:$l}
#	for init in ${inits[@]}
#	do
#		./fold.sh test/$alg-$f-$init.csv $options -f $file -init $init
#	done
#done
