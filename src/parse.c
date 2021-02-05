#include <stdio.h>
#include "lexer.h"

extern const char* CC_TYPES[];

void parse(token* t) {
	for (int i = 0; t[i+1].i != Eof; i++) {
        if (t[i].i >= 128) {
            printf("%s \'%s\'\n", CC_TYPES[t[i].i - 128], t[i].s);
        }
    }
}