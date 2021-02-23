#!/bin/bash
clang -fsyntax-only -Xclang -dump-tokens "$*" 2>&1 #| sed -e 's/[[a-Z][z-Z].*]//' -e 's/Loc\=.*//'
