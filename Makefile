compile:
	clang-12 -std=c99 -Wall -pedantic *.c -L. -lruntime

run:
	./a.out -p 5 --mutexl