# Makefile de exemplo (Manual do GNU Make)
     
CFLAGS = -Wall -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = *.o 
     
mundo: mundo.o liblef.o libconjunto.o libfila.o
	$(CC) -o mundo mundo.o liblef.o libconjunto.o libfila.o $(LDFLAGS)

libfila.o: libfila.c
	$(CC) -c $(CFLAGS) libfila.c

libconjunto.o: libconjunto.c
	$(CC) -c $(CFLAGS) libconjunto.c

liblef.o: liblef.c
	$(CC) -c $(CFLAGS) liblef.c

mundo.o: mundo.c
	$(CC) -c $(CFLAGS) mundo.c

clean:
	rm -f $(objects) mundo


