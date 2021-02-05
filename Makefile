INCLUDE=-Iinclude
CFLAGS=-Wall -O2 -std=c99
CFILES=$(patsubst src/%.c,%.o,$(wildcard src/*.c))

cc: $(CFILES)
	$(CC) $(INCLUDE) $^ -o $@ $(CFLAGS)
%.o: src/%.c include/%.h
	$(CC) $(INCLUDE) -c $< -o $@ $(CFLAGS)
clean:
	rm -f cc *.o *.s
