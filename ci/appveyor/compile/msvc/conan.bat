pushd %cd% &&^
cmake .. -DRTERM_BUILD_TESTS=ON -DRTERM_EXTERNAL_LIBS=%RTERM_LIBS% -G "Visual Studio 15 2017 Win64" &&^
cmake --build . --config Release &&^
popd ||^
echo Build failed!
