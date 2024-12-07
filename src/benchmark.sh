#!/usr/bin/env bash

make all
echo "Benchmarking all days, avg and best times of 100 repetitions in microseconds:"
echo -e "\t\t\tAverage \tBest"\

numFiles=$(ls -1q day?? | wc -l)
for i in $(seq -f "%02g" 1 $numFiles);
do
    echo "Day $i:"
    part1best=0
    part2best=0
    part1avg=0
    part2avg=0
    for j in $(seq 1 100);
    do
        ./day$i ../input/day$i.txt > ./tmp.txt
        val=$(cat tmp.txt | grep "1 Time" | awk '{print $4}')
        part1avg=$((val + part1avg))
        if [ $val -lt $part1best ] || [ $part1best -eq "0" ]; then
            part1best=$val
        fi
        val=$(cat tmp.txt | grep "2 Time" | awk '{print $4}')
        part2avg=$((val + part2avg))
        if [ $val -lt $part2best ] || [ $part2best -eq "0" ]; then
            part2best=$val
        fi
    done
    part1avg=$((part1avg / 100))
    part2avg=$((part2avg / 100))
    echo -e "\tPart 1: \t$part1avg us \t\t$part1best us"
    if [ $i -eq "6" ]; then
        echo -e "\tPart 2: \t$part2avg us \t$part2best us"
    else
        echo -e "\tPart 2: \t$part2avg us \t\t$part2best us"
    fi
done

rm tmp.txt
# make clean