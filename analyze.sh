#!/bin/sh

for i in $(seq 1 10)
do
	echo ./cv-fast --init-pattern xorshift128plus --init-seed $i --file-out /dev/null --cpus 1
done
for i in $(seq 1 10)
do
	echo ./cv-fast --init-pattern xorshift128plus --init-seed $i --file-out /dev/null --cpus 4
done
