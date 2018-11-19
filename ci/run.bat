SET STARTDIR="%cd%"
cd .. &&^
mkdir bulid &&^
cd build &&^
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DRTERM_ADD_TEST_TARGET=ON &&^
cmake --build . --parallel 4 &&^
copy librterm.dll test &&^
cd test &&^
benchmark.exe ||^
pause
cd %STARTDIR%
