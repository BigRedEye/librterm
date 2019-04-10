pushd %cd% &&^
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat" &&^
dumpbin /DEPENDENTS build\bin\Release\benchmark.exe &&^
build\bin\Release\benchmark.exe build\bin\resources 300 500 &&^
popd ||^
echo Run failed!
