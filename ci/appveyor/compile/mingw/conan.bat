pushd %cd% &&^
cmake .. -DCMAKE_BUILD_TYPE=Release -DRTERM_EXTERNAL_LIBS=%RTERM_LIBS% -DRTERM_BUILD_TESTS=ON -G "MinGW Makefiles" &&^
cmake --build . --config Release &&^
mkdir bin\Release &&^
move bin\*.exe bin\Release &&^
popd ||^
echo Build failed!
