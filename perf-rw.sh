#!/bin/bash
#arg: file

make -s clean
make -s $1 2> /dev/null

echo "n_threads,time1,time2,time3,time4,time5" > measure.csv

for i in 1 8 16 32 64; do
	echo -n "$i" >> measure.csv
	for j in 1 2 3 4 5; do
		timeval=`/usr/bin/time -f %e $1 -r $i -w $i 2>&1`
		echo -n ",$timeval" >> measure.csv
	done
	echo "" >> measure.csv
done
