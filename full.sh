#!/bin/bash

echo "Full Factorial Experiment"
echo "Running..."

endOut="output.txt"

echo "bin/solver $@" > $endOut

for j in {1..3}
do
	for i in {0..9}
	do
		echo -en "\rSet #$((i+1)) - Run #$j"
		bin/solver $@ -10fcv $i -nv >> $endOut
	done
done

echo -e "\rResults saved at $endOut"
echo "Done."
