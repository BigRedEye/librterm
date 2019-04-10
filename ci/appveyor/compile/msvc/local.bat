pushd %cd% &&^
mkdir build &&^
cd build &&^
cmake .. -DRTERM_BUILD_TESTS=ON -DRTERM_EXTERNAL_LIBS=%RTERM_LIBS% -G "Visual Studio 15 2017" -A x64 &&^
cmake --build . --config Release &&^
popd ||^
echo Build failed!
