#!/bin/bash
#arg: dossier

make -C $1 -s clean

echo "n_threads,time1,time2,time3,time4,time5" > measure.csv

for i in 1 8 16 32 64; do
	echo -n "$1" >> measure.csv
	for _ in {1...5}; do
		echo "Essai $j avec $i threads"
		time_val=$(/usr/bin/time -f %e make -C $1 -j $i -s all >/dev/null | tail -1)
		echo -n ",$time_val" >> measure.csv
		make -C $1 -s clean
	done
	echo "" >> measure.csv
done