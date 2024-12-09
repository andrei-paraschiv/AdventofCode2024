#!/usr/bin/env bash

make all
echo -e "Benchmarking all days, avg and best times of 100 repetitions in microseconds:"
echo -ne "Progress: Day 01"
echo "Day;Part;Average;Best"\ > tmp2.txt

numFiles=$(ls -1q day?? | wc -l)
for i in $(seq -f "%02g" 1 $numFiles);
do
    echo -ne "\b\b$i"
    echo "Day $i:" >> tmp2.txt
    part1best=0
    part2best=0
    part1avg=0
    part2avg=0
    for j in $(seq 1 100);
    do
        ./day$i ../input/day$i.txt > ./tmp1.txt
        val=$(cat tmp1.txt | grep "1 Time" | awk '{print $4}')
        part1avg=$((val + part1avg))
        if [ $val -lt $part1best ] || [ $part1best -eq "0" ]; then
            part1best=$val
        fi
        val=$(cat tmp1.txt | grep "2 Time" | awk '{print $4}')
        part2avg=$((val + part2avg))
        if [ $val -lt $part2best ] || [ $part2best -eq "0" ]; then
            part2best=$val
        fi
    done
    part1avg=$((part1avg / 100))
    part2avg=$((part2avg / 100))
    echo " ;Part 1:;$part1avg us;$part1best us" >> tmp2.txt
    echo " ;Part 2:;$part2avg us;$part2best us" >> tmp2.txt
done

echo -e "\r\e[K"
column -t -s ";" tmp2.txt

rm tmp1.txt tmp2.txt
# make clean