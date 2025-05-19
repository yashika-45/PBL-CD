#!/bin/bash


win_bison -d phase2.y
win_flex phase2.l
gcc lex.yy.c phase2.tab.c -o phase2.exe
phase2.exe phase2_input.c > ../output/result.txt
cat ../output/result.txt

