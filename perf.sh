#!/bin/bash
#arg: filename => read-write | prod-conso | philosophes

make -s $1 2> /dev/null

if [ $1 = "read-write" ]; then
	file="read-write.csv"
fi
if [ $1 = "prod-conso" ]; then
	file="prod-conso.csv"
fi
if [ $1 = "philosophes" ]; then
	file="philosophes.csv"
fi

echo "n_threads,time1,time2,time3,time4,time5" > $file

for i in 1 8 16 32 64; do
	echo -n "$i" >> $file
	for j in 1 2 3 4 5; do
		if [ $1 = "read-write" ]; then
			timeval=`/usr/bin/time -f %e ./$1 -r $i -w $i 2>&1`
		fi
		if [ $1 = "prod-conso" ]; then
			timeval=`/usr/bin/time -f %e ./$1 -p $i -c $i 2>&1`
		fi
		if [ $1 = "philosophes" ]; then
			timeval=`/usr/bin/time -f %e ./$1 -j $i -n 4 2>&1`
		fi
		echo -n ",$timeval" >> $file
	done
	echo "" >> $file
done
