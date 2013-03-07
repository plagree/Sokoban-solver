#Loop testing different boards on server for statistics

#!/bin/bash

let "boards_solved = 0"
let "zero = 0"

for i in `seq 1 100`
do
	timeout 30 ./main dd2380.csc.kth.se 5032 $i
	if [ $? -eq $zero ]
	then
		let "boards_solved += 1"
	fi
done

echo "We solved $boards_solved boards"
