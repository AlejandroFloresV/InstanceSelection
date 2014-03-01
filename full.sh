#!/bin/bash

echo "Full Factorial Experiment"
echo "Running..."

endOut="output.txt"

echo "bin/solver $@" > $endOut

for i in {0..9}
do
	for j in {1..5}
	do
		echo -en "\rSet #$((i+1)) - Run #$j"
		bin/solver $@ -10fcv $i -nv >> $endOut
		sleep 2
	done
done

echo -e "\rResults saved at $endOut"
echo "Done."
