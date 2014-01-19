#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Usage: <algorithm> <set>"
	exit $E_BADARGS
fi

echo "Full Factorial Experiment"
echo "Running..."

for i in {0..9}
do
	for j in {1..5}
	do
		echo -en "\rSet #$i - Run #$j"
		(time ./run $1 data/$2.norm $i) >> output/rawout 2>&1
	done
done

echo -e "\rParsing output..."
output/parse < output/rawout > output/out
rm output/rawout
echo "Done."
