pushd %cd% &&^
mkdir build &&^
cd build &&^
cmake .. -DRTERM_BUILD_TESTS=ON -DRTERM_EXTERNAL_LIBS=%RTERM_LIBS% -DRTERM_SDL2_PATH=%DEPS_PATH% -G "Visual Studio 15 2017 Win64" &&^
cmake --build . --config Release &&^
popd ||^
echo Build failed!
