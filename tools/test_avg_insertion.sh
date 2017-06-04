#!/bin/sh

sum="0"
while IFS='' read -r line
do
	cur=`echo "$line" | cut -f2`
	# echo $cur
	echo $sum
	sum=`bc <<< "$sum + $cur"`
	echo $sum
done < data/insertion.oo_10000000
avg=`bc <<< "$sum / 10000000"`
echo $avg > data/insertion_avg.oo_10000000


# sum=0
# while IFS='' read -r line
# do
# 	cur="$(echo "$line" | cut -f2)"
# 	sum="$(bc <<< "$sum + $cur")"
# done < data/insertion.do0_10000000
# avg="$(bc <<< "$sum / 10000000")"
# echo $avg > data/insertion_avg.do0_10000000
#
#
# sum=0
# while IFS='' read -r line
# do
# 	cur="$(echo "$line" | cut -f2)"
# 	sum="$(bc <<< "$sum + $cur")"
# done < data/insertion.do1_10000000
# avg="$(bc <<< "$sum / 10000000")"
# echo $avg > data/insertion_avg.do1_10000000
