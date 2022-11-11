# Makefile Pedro Amaral

CC = gcc
CFLAGS = -pthread    # regras de compilação
LIBS = -lm    # bibliotecas a ligar

# arquivos objeto
OBJ = jaro-winkler.o batch_jaro_winkler.o

output: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o output

jaro-winkler.o: jaro-winkler.c
	$(CC) $(CFLAGS) -c jaro-winkler.c

batch_jaro_winkler.o: batch_jaro_winkler.c batch_jaro_winkler.h batch_jaro_winkler_internal.h batch_jaro_winkler_runtime.h uthash.h
	$(CC) $(CFLAGS) -c batch_jaro_winkler.c

clean:
	rm *.o