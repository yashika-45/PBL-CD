@echo off
setlocal

REM Compile semantic analyzer
gcc -o semantic.exe main.c semantic.c apt.c ast.c symbol_table.c parser_semantic.tab.c lex.yy.c -I.

if %errorlevel% neq 0 (
    echo ❌ Compilation failed.
    pause
    exit /b 1
)

REM Run semantic analyzer on input.c located in ../temp
if not exist "..\temp\input.c" (
    echo ❌ input.c not found in ../temp folder.
    pause
    exit /b 1
)

echo 🚀 Running semantic analysis...
semantic.exe ..\temp\input.c

endlocal
