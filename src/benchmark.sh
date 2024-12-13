#!/usr/bin/env bash

if [ $# -gt 0 ]; then
    day=$(expr $1 + 0 2>/dev/null)
    if [ $# -gt 1 ] || ([ $# -eq 1 ] && [ $day -gt "25" -o $day -eq "0" ] 2>/dev/null || ! expr $day : '[0-9]\{1,2\}$' &>/dev/null); then
        echo "Usage:"
        echo -e "\t$0 to benchmark all days"
        echo -e "\t$0 [day] to benchmark a specific day, day must be a number between 1 and 25 inclusive"
        exit
    fi
fi

if [ $# -eq "1" ]; then
    dayPadded=$day
    if [ $day -lt "10" ]; then
        dayPadded="0$day"
    fi
    make "day$dayPadded" > /dev/null

    echo -e "Benchmarking day $day, avg and best times of 100 repetitions in microseconds:\n"
    echo "Day;Part;Average;Best" > tmp2.txt

    part1best=0
    part2best=0
    part1avg=0
    part2avg=0
    for j in $(seq 1 100);
    do
        ./day$dayPadded ../input/day$dayPadded.txt > ./tmp1.txt
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
    echo "$day;1;$part1avg us;$part1best us" >> tmp2.txt
    echo "$day;2;$part2avg us;$part2best us" >> tmp2.txt
    column -t -s ";" tmp2.txt

    rm tmp1.txt tmp2.txt
    make clean > /dev/null
    exit
fi

make all > /dev/null
echo -e "Benchmarking all days, avg and best times of 100 repetitions in microseconds:"
echo -ne "Progress: Day 01"
echo "Day;Part;Average;Best" > tmp2.txt

numFiles=$(ls -1q day?? | wc -l)
for i in $(seq -f "%02g" 1 $numFiles);
do
    echo -ne "\b\b$i"
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
    echo "$i;1;$part1avg us;$part1best us" >> tmp2.txt
    echo "$i;2;$part2avg us;$part2best us" >> tmp2.txt
done

echo -e "\r\e[K"
column -t -s ";" tmp2.txt

rm tmp1.txt tmp2.txt
make clean > /dev/null