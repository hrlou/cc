INCLUDE=-Iinclude
CFLAGS=-Wall -O2 -std=c99
PREFIX=/usr/local
CFILES=main.o lexer.o

cc: $(CFILES)
	$(CC) $(INCLUDE) $^ -o $@ $(CFLAGS)
%.o: src/%.c include/%.h
	$(CC) $(INCLUDE) -c $< -o $@ $(CFLAGS)
clean:
	rm -f cc *.o *.s
