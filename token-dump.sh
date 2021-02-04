#!/bin/sh
clang -fsyntax-only -Xclang -dump-tokens "$*" 2>&1
