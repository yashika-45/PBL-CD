@echo off

REM Kill previous instance if running (use with caution)
taskkill /IM phase2.exe /F >nul 2>&1

REM Clean old generated files
del lex.yy.c >nul 2>&1
del phase2.exe >nul 2>&1

REM Rebuild lexer and parser
win_bison -d phase2.y
win_flex phase2.l
gcc phase2.tab.c lex.yy.c ast.c -o phase2.exe

REM Create output folder if needed
if not exist ..\output mkdir ..\output

REM Run analyzer on input
phase2.exe temp\input.c > ..\output\result.txt

REM Show the output
type ..\output\result.txt
