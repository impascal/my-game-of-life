:: script setup
@ECHO off
CLS

:: some useful variables
set CC=gcc
set CFLAG="-std=c99 -Wall"

set SRC=.\src
set SRCS=%SRC%\*.c
set MAIN_SRC=%SRC%\main.c
set IDIR=.\include
set INCLUDE=%IDIR%\*.h
set LDIR=.\lib
:: enter the name of your libs here separated by a "-l" flag
set LIBS=-l SDL2 
set ODIR=.\obj
set OBJ=.\obj\*.o
set MAIN_OBJ=.\obj\main.o
set BDIR=.\bin
set BIN=.\bin\gol.exe

:: decides what to do based on user arguments
:main
    echo perahhh's build.bat script running...

    if "%1" == "run" (
        CALL :run
        EXIT 0
    )
    if "%1" == "clean" (
        CALL :clean
        EXIT 0
    )

    if NOT exist %BDIR% ( 
        echo Creating %BDIR%
        mkdir %BDIR% 
    )
    if NOT exist %ODIR% ( 
        echo Creating %ODIR%
        mkdir %ODIR% )

    CALL :clean
    CALL :compile
    CALL :link
EXIT /B 0

:compile
    echo Compiling!
    %CC% --version
    for %%i in (%SRCS%) do (
        if NOT %%i == %MAIN_SRC% (
            %CC% "%CFLAG%" -o %ODIR%\%%~ni.o -c %%i -I %IDIR%
        )
    )
    %CC% "%CFLAG%" -o %MAIN_OBJ% -c %MAIN_SRC% -I %IDIR%

    @ECHO off
EXIT /B 0

:link
    echo Linking!
    @ECHO on
    %CC% %OBJ% -L %LDIR% %LIBS% -o %BIN%
    @ECHO off
EXIT /B 0

:run
    echo Launching the program!
    @ECHO on
    %BIN%
    @ECHO off
EXIT /B 0

:clean
    echo Cleaning objs and binaries!
    @ECHO on
    del %BIN% %OBJ%
    @ECHO off
EXIT /B 0

EXIT 0