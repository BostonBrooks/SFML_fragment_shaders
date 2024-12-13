echo off
echo Look, a Tortoise:
echo [32m
echo:
echo   _____     ____
echo  /      \  ^|  o ^|
echo ^|        ^|/ ___\^|
echo ^|_________/
echo ^|_^|_^| ^|_^|_^|
echo /////////////////////////////////////////////////////////////
echo [0m
echo:
echo Attempting to compile:
SET PATH=C:\winlibs\mingw64\bin
gcc   -D SFML_STATIC -I ../^
       ../test/test_Kernels.c^
       ../engine/maths/bbGPU_Interpolation.c^
  -lcsfml-system -lcsfml-graphics -lcsfml-window -g -lm -w^
    -o ./a.exe
IF %errorlevel% == 0 (  a.exe)^
 ELSE (echo compile failed!)