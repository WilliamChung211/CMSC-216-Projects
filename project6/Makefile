#Name: William Chung
#Discusion Number:0101 

CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -fstack-protector-all -Werror -Wshadow

all: program1.x main.x tester.x

program1.x: row-compare.o program1.o
	gcc program1.o row-compare.o -o program1.x

main.x: main.o range-fill.o array-print.o
	gcc main.o range-fill.o array-print.o -o main.x

tester.x: array-sum.o range-fill.o row-sum.o tester.o
	gcc array-sum.o range-fill.o row-sum.o tester.o -o tester.x

program1.o: program1.c row-compare.h
	$(CC) $(CFLAGS) -c program1.c
row-compare.o: row-compare.c row-compare.h
	$(CC) $(CFLAGS) -c row-compare.c

main.o:	main.c  range-fill.h array-print.h
	$(CC) $(CFLAGS) -c main.c

array-print.o: array-print.h array-print.c

range-fill.o: range-fill.h range-fill.c
	$(CC) $(CFLAGS) -c range-fill.c

array-sum.o: array-sum.c array-sum.h
	$(CC) $(CFLAGS) -c array-sum.c
row-sum.o: row-sum.h row-sum.c
	$(CC) $(CFLAGS) -c row-sum.c

tester.o: range-fill.h array-sum.h row-sum.h tester.c
	$(CC) $(CFLAGS) -c tester.c
clean:
	rm -f *.o *.x
