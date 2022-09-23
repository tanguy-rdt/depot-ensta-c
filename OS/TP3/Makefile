CC=gcc
CFLAGS=-Wall -lrt 
LDFLAGS=
EXEC=nbOctets

all: $(EXEC)

nbOctets: partage.o nbOctets.o
	$(CC) -o nbOctets partage.o nbOctets.o $(LDFLAGS)

partage.o: partage.c
	$(CC) -o partage.o -c partage.c $(CFLAGS)

nbOctets.o: nbOctets.c partage.h
	$(CC) -o nbOctets.o -c nbOctets.c $(CFLAGS)

clean:
	rm -rf *.o nbOctets