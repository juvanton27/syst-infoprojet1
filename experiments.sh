#!/bin/bash
#arg: filename => read-write[-test-and[-test-and]-set || sem] | prod-conso[-test-and[-test-and]-set || sem] | philosophes[-test-and[-test-and]-set || sem]

make -s $1 2> /dev/null

file="$1.csv"

echo "n_threads,time1,time2,time3,time4,time5" > $file

for i in 1 8 16 32 64; do
	echo -n "$i" >> $file
	for j in 1 2 3 4 5; do
		# For simple c
		if [ $1 = "read-write" ]; then
			timeval=`/usr/bin/time -f %e ./$1 -r $i -w $i 2>&1`
		fi
		if [ $1 = "prod-conso" ]; then
			timeval=`/usr/bin/time -f %e ./$1 -p $i -c $i 2>&1`
		fi
		if [ $1 = "philosophes" ]; then
			timeval=`/usr/bin/time -f %e ./$1 -j $i -n 4 2>&1`
		fi
		# For test-and-set and test-and-test-and-set
		if [ $1 = "read-write-test-and-set" ] || [ $1 = "read-write-test-and-test-and-set" ] || [ $1 = "read-write-sem" ]; then
			if [ $i = 32 ] || [ $i = 64 ]; then
				timeval=`/usr/bin/time -f %e ./$1 -r $i -w $i 2>&1`
			fi
		fi
		if [ $1 = "prod-conso-test-and-set" ] || [ $1 = "prod-conso-test-and-test-and-set" ] || [ $1 = "prod-conso-sem" ]; then
			if [ $i = 32 ] || [ $i = 64 ]; then
				timeval=`/usr/bin/time -f %e ./$1 -p $i -c $i 2>&1`
			fi
		fi
		if [ $1 = "philosophes-test-and-set" ] || [ $1 = "philosophes-test-and-test-and-set" ]; then
			if [ $i = 32 ] || [ $i = 64 ]; then
				timeval=`/usr/bin/time -f %e ./$1 -j $i -n 4 2>&1`
			fi
		fi
		echo -n ",$timeval" >> $file
	done
	echo "" >> $file
done
