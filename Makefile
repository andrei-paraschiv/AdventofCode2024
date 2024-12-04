all: c_all rs_all

c_all: day01 day02 day03 day04

rs_all: day01_rs

day01: day01.c
	gcc day01.c -o day01

day02: day02.c
	gcc day02.c -o day02

day03: day03.c
	gcc day03.c -o day03

day04: day04.c
	gcc day04.c -o day04

day01_rs: day01.rs
	rustc day01.rs -o da0y1_rs

clean:
	rm -f day01 day02 day03 day04