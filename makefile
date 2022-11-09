CC = gcc
CFLAGS = -Wall -std=c99

lattes: lattes.o libcoleta.o libescrita.o
	$(CC) $(CFLAGS) lattes.o libcoleta.o libescrita.o -o lattes

lattes.o: lattes.c
	$(CC) $(CFLAGS) -c lattes.c

libcoleta.o: libcoleta.c libcoleta.h
	$(CC) $(CFLAGS) -c libcoleta.c 

libescrita.o: libescrita.c libescrita.h
	$(CC) $(CFLAGS) -c libescrita.c

clean:
	rm *.o
