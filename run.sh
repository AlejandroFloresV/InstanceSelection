#!/bin/bash

echo "Full Factorial Experiment"
echo "Running..."

mkdir -p output
rm -f output/result.rawout

for i in {0..9}
do
	for j in {1..5}
	do
		echo -en "\rSet #$((i+1)) - Run #$j"
		(time bin/solver $@ -10fcv $i -nv) >> output/result.rawout 2>&1
		sleep 2
	done
done

echo -e "\rParsing output..."
bin/parseOutput < output/result.rawout > output/result.out
rm output/result.rawout
echo "Saved at output/result.out"
echo "Done."
