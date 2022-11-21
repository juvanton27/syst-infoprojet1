#!/bin/bash
#arg: file

make -s clean
make -s $1 2> /dev/null

if [ $1 = "read-write" ]; then
	file="measure-rw.csv"
fi

echo "n_threads,time1,time2,time3,time4,time5" > $file

for i in 1 8 16 32 64; do
	echo -n "$i" >> $file
	for j in 1 2 3 4 5; do
		if [ $1 = "read-write" ]; then
			timeval=`/usr/bin/time -f %e ./$1 -r $i -w $i 2>&1`
		fi
		echo -n ",$timeval" >> $file
	done
	echo "" >> $file
done
