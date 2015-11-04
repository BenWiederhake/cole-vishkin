#!/bin/sh

for cpus in $(seq 1 5)
do
	for i in $(seq 1 10)
	do
		echo @./cv-fast --init-pattern xorshift128plus --init-seed $i --file-out /dev/null --cpus $cpus --format tdl
	done
	echo "#"
done
