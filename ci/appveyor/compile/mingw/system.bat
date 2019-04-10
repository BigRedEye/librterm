pushd %cd% &&^
mkdir build &&^
cd build &&^
cmake .. -DCMAKE_BUILD_TYPE=Release -DRTERM_EXTERNAL_LIBS=%RTERM_LIBS% -DRTERM_SDL2_PATH=%DEPS_PATH% -DCMAKE_SH="CMAKE_SH-NOTFOUND" -DRTERM_BUILD_TESTS=ON -G "MinGW Makefiles" &&^
cmake --build . --config Release &&^
mkdir bin\Release &&^
move bin\*.exe bin\Release &&^
popd ||^
echo Build failed!
