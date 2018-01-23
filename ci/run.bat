SET STARTDIR="%cd%"
cd .. &&^
cmake . -G "MinGW Makefiles" &&^
mingw32-make &&^
copy librterm.dll test &&^
cd test &&^
cmake . -G "MinGW Makefiles" &&^
mingw32-make &&^
benchmark.exe ||^
pause
cd %STARTDIR%
