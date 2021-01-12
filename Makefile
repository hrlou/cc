CC = gcc
CFLAGS = "-Iinclude" "-Wall" "-O2" "-std=c99"
PREFIX = /usr/local

cc: lexer.o main.o
	$(CC) $(CFLAGS) $^ -o $@
	
lexer.o: src/lexer.c
	$(CC) $(CFLAGS) -c $< -o $@

main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f cc *.o *.s
