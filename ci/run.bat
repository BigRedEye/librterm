SET STARTDIR="%cd%"
cd .. &&^
cmake . -G "MinGW Makefiles" &&^
mingw32-make &&^
copy librterm.dll demo &&^
cd demo &&^
cmake . -G "MinGW Makefiles" &&^
mingw32-make &&^
demo.exe ||^
pause
cd %STARTDIR%
