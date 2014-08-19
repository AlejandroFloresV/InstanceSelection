#!/bin/bash

alg=PSO
options="-alg $alg -iter 1000 -pop 5 -pso 5 -w 0.9 -c1 0.1 -c2 0.1 -vmax 0.2 -alpha 0.5"

for file in `ls data/*.csv`
do
	l=`expr ${#file} - 9`
	f=${file:5:$l}
	./fold.sh test/$alg-$f-Unif50.csv $options -f $file
done
