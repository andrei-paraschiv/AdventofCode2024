all: day1

day1: day1.c
	gcc day1.c -o day1

clean:
	rm -f day1