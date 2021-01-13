INCLUDE = "-Iinclude" 
CFLAGS = "-Wall" "-O2" "-std=c99"
CC = gcc $(INCLUDE)$(CFLAGS)
PREFIX = /usr/local

cc: main.o lexer.o
	$(CC) $^ -o $@
	
lexer.o: src/lexer.c
	$(CC) -c $< -o $@

main.o: src/main.c
	$(CC) -c $< -o $@

clean:
	rm -f cc *.o *.s
