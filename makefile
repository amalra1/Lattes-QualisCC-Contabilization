CC = gcc
CFLAGS = -Wall -std=c99

# Arquivos objeto:
OBJ = lattes.o libcoleta.o libescrita.o levenshtein.o

lattes: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o lattes

lattes.o: lattes.c
	$(CC) $(CFLAGS) -c lattes.c

libcoleta.o: libcoleta.c libcoleta.h levenshtein.h 
	$(CC) $(CFLAGS) -c libcoleta.c

libescrita.o: libescrita.c libescrita.h
	$(CC) $(CFLAGS) -c libescrita.c

levenshtein.o: levenshtein.c levenshtein.h
	$(CC) $(CFLAGS) -c levenshtein.c

clean:
	rm *.o
	
purge: clean
	rm -f lattes
