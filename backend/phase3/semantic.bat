@echo off

REM Compile semantic analyzer
gcc -o semantic.exe main.c semantic.c apt.c ast.c parser_semantic.tab.c lex.yy.c -I.

if %errorlevel% neq 0 (
    echo Compilation failed.
    pause
    exit /b 1
)

REM Run semantic analyzer on input.c located in temp folder
semantic.exe ..\temp\input.c
