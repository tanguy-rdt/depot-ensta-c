CC = gcc
CFLAGS = -Wall
PROGRAMS = sema rdv1 rdv2


all: $(PROGRAMS)

sema: sema.c
	$(CC) sema.c -o sema $(CFLAGS)

rdv1: rdv1.c
	$(CC) rdv1.c -o rdv1 $(CFLAGS)

rdv2: rdv2.c
	$(CC) rdv2.c -o rdv2 $(CFLAGS)

clean:
	rm -rf $(PROGRAMS)
