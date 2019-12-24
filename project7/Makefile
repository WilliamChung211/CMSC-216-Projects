#Name: William Chung
#ID: Wchung1
#UID: 116609968
#Discusion Number:0101

CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -fstack-protector-all -Werror -Wshadow

all: public01.x public02.x public03.x public04.x public05.x public06.x public07.x public08.x public09.x public10.x



public01.x: public01.o list.o
	$(CC) list.o public01.o -o public01.x

public02.x: public02.o list.o
	$(CC) list.o public02.o -o public02.x

public03.x: public03.o list.o
	$(CC) list.o public03.o -o public03.x

public04.x: public04.o list.o
	$(CC) list.o public04.o -o public04.x

public05.x: public05.o list.o
	$(CC) list.o public05.o -o public05.x

public06.x: public06.o list.o
	$(CC) list.o public06.o -o public06.x

public07.x: public07.o list.o
	$(CC) list.o public07.o -o public07.x

public08.x: public08.o list.o
	$(CC) list.o public08.o -o public08.x

public09.x: public09.o list.o
	$(CC) list.o public09.o -o public09.x

public10.x: public10.o list.o
	$(CC) list.o public10.o -o public10.x

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

public01.o: public01.c list.h
	$(CC) $(CFLAGS) -c public01.c

public02.o: public02.c list.h
	$(CC) $(CFLAGS) -c public02.c

public03.o: public03.c list.h
	$(CC) $(CFLAGS) -c public03.c

public04.o: public04.c list.h
	$(CC) $(CFLAGS) -c public04.c

public05.o: public05.c list.h
	$(CC) $(CFLAGS) -c public05.c

public06.o: public06.c list.h
	$(CC) $(CFLAGS) -c public06.c

public07.o: public07.c list.h
	$(CC) $(CFLAGS) -c public07.c

public08.o: public08.c list.h
	$(CC) $(CFLAGS) -c public08.c

public09.o: public09.c list.h
	$(CC) $(CFLAGS) -c public09.c

public10.o: public10.c list.h
	$(CC) $(CFLAGS) -c public10.c
clean:
	rm -f *.o *.x
