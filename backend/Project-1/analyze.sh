#!/bin/bash

# Compile the lexical analyzer using flex and GCC
win_flex lexer.l
gcc lex.yy.c

# Run the lexical analyzer on the input code (input.c)
a.exe testcase-1.c > ../output/result.txt

# Print the results to the terminal
cat ../output/result.txt
