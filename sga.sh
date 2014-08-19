#!/bin/bash

alg=SGA
options="-alg $alg -iter 1000 -pop 30 -cross 1.0 -bit 0.5"

for file in `ls data/*.csv`
do
	l=`expr ${#file} - 9`
	f=${file:5:$l}
	./fold.sh test/$alg-$f-Unif50.csv $options -f $file
done
