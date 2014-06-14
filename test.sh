#!/bin/bash

alg=GGA
files=("iris" "wdbc" "banana")
iters=(100 1000 10000)
pops=(10 20 30 40 50)
cprob=("0.5" "0.6" "0.75" "0.9" "1.0")

for file in ${files[@]}
do
	for iter in ${iters[@]}
	do
		for pop in ${pops[@]}
		do
			for cross in ${cprob[@]}
			do
				./full.sh test/$alg-$file-$iter-$pop-$cross.csv -f data/${file}.norm -alg $alg -iter $iter -pop $pop -cross $cross
			done
		done
	done
done
