pushd %cd% &&^
set PATH=%PATH:C:\Program Files\Git\usr\bin;=% &&^
set PATH=%PATH%;%DEPS_PATH%\bin &&^
popd ||^
echo Build failed!
