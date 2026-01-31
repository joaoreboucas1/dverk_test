main: main.c
	gcc -Wall -Wextra -Wno-compare-reals dverk.f main.c -o main -lgfortran -lm -lgsl -lgslcblas -ggdb

clean:
	rm ./main