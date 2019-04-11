pushd .. &&^
mkdir %DEPS_PATH% &&^
appveyor DownloadFile https://www.libsdl.org/release/SDL2-devel-2.0.9-VC.zip &&^
7z x SDL2-devel-2.0.9-VC.zip &&^
xcopy SDL2-2.0.9 %DEPS_PATH% /s/e/y &&^
appveyor DownloadFile https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.0.15-VC.zip &&^
7z x SDL2_ttf-devel-2.0.15-VC.zip &&^
xcopy SDL2_ttf-2.0.15 %DEPS_PATH% /s/e/y &&^
popd ||^
echo Build failed!
