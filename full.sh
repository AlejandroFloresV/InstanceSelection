#!/bin/bash

echo "Full Factorial Experiment"
echo "Running..."

rawOut="rawout.txt"
endOut="output.txt"
rm -f $rawOut

for i in {0..9}
do
	for j in {1..5}
	do
		echo -en "\rSet #$((i+1)) - Run #$j"
		(time bin/solver $@ -10fcv $i -nv) >> $rawOut 2>&1
		sleep 2
	done
done

echo -e "\rParsing output..."
echo "bin/solver $@" >> $endOut
bin/parseOutput < $rawOut >> $endOut
rm $rawOut
echo "Saved at $endOut"
echo "Done."
