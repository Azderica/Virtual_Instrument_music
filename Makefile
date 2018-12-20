all : test

test : test.c
	gcc -pthread test.c -o test
