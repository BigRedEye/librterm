pushd %cd% &&^
echo "Downloading conan..." &&^
set PATH=%PATH%;%PYTHON%/Scripts/ &&^
pip.exe install conan &&^
conan --version &&^
conan user &&^
popd ||^
echo Build failed!
