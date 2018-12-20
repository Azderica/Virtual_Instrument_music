all : test

test : test.c
	gcc test.c -lpthread -lcurses -o test
