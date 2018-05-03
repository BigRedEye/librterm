cd ..
set MINGW=C:\mingw-w64\x86_64-7.2.0-posix-seh-rt_v5-rev\mingw64
set INSTALL_PATH=%MINGW%\x86_64-w64-mingw32\
copy librterm.dll %INSTALL_PATH%\bin\
copy librterm.dll.a %INSTALL_PATH%\lib\
mkdir %INSTALL_PATH%\include\rterm
copy include %INSTALL_PATH%\include\rterm\
cd ci