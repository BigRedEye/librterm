SET STARTDIR="%cd%"
cd .. &&^
cmake . -G "MinGW Makefiles" &&^
mingw32-make &&^
cd test &&^
benchmark.exe ||^
pause
cd %STARTDIR%
