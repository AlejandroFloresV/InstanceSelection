#!/bin/bash

alg=PSO
options="-alg $alg -iter 1000 -pop 30 -pso 5 -w 0.9 -c1 0.1 -c2 0.1 -vmax 0.2"

for file in `ls data/*.csv`
do
	l=`expr ${#file} - 9`
	f=${file:5:$l}
	./fold.sh test/$alg-$f-$1.csv $options -f $file -init $1
done
