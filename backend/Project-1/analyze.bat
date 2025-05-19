@echo off

REM Set output executable name explicitly
set EXEC=lexer.exe

REM Compile only if needed
if not exist %EXEC% (
    echo Compiling lexer...
    win_flex lexer.l
    gcc lex.yy.c -o %EXEC%
)

REM Create output directory if it doesn't exist
if not exist ..\output mkdir ..\output

REM Run the analyzer using the input provided by Flask
%EXEC% < ..\temp\input.c > ..\output\result.txt

REM Print output to console (Flask captures this)
type ..\output\result.txt
