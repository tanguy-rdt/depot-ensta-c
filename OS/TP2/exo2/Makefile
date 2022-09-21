CC = gcc
CFLAGS = -Wall
PROGRAMS = sema lecteur redacteur


all: $(PROGRAMS)

sema: sema.c
	$(CC) sema.c -o sema $(CFLAGS)

lecteur: lecteur.c
	$(CC) lecteur.c -o lecteur $(CFLAGS)

redacteur: redacteur.c
	$(CC) redacteur.c -o redacteur $(CFLAGS)

clean:
	rm -rf $(PROGRAMS)