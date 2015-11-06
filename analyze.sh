#!/bin/sh

echo '##CV_ANALYZE_OPTS="--init-pattern xorshift128plus --file-out /dev/null --format tdl --length 536870912 --length-force"'
for cpus in $(seq 1 5)
do
	for i in $(seq 1 10)
	do
		echo @./cv-fast --init-seed $i --cpus $cpus '${CV_ANALYZE_OPTS}'
	done
	echo "#"
done
