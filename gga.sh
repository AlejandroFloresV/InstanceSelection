#!/bin/bash

alg=GGA
options="-alg $alg -iter 1000 -pop 50 -cross 0.9 -alpha 0.5"

for file in `ls data/*.csv`
do
	l=`expr ${#file} - 9`
	f=${file:5:$l}
	./fold.sh test/$alg-$f-Unif50.csv $options -f $file
done
