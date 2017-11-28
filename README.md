# rterm [![Build Status](https://travis-ci.com/BigRedEye/rterm.svg?token=HabA2F1p73cnpyrz3Jdj&branch=master)](https://travis-ci.com/BigRedEye/rterm)
#


Minimalistic terminal emulator written on C++ and SDL2.
    
## Requirements ##

+ [SDL2](https://www.libsdl.org/download-2.0.php)
+ [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)
+ [SDL2_image](https://www.libsdl.org/projects/SDL_image/)
+ [cmake](https://cmake.org/)

## Building ##
### Linux ###
+ Install the latest version of [SDL2](https://www.libsdl.org/download-2.0.php),
[SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/),
[SDL2_image](https://www.libsdl.org/projects/SDL_image/) and
[cmake](https://cmake.org/).

+ clone this repo
```
git clone https://github.com/BigRedEye/rterm.git
cd rterm
```

+ build
```
cmake .
make
```

+ install the library
```
sudo install librterm.so /usr/local/lib
```

+ it's highly recommend to build example programs
```
cd demo
cmake .
make
./demo
```

### Windows (MinGW) ###
+ Install the latest version of [SDL2](https://www.libsdl.org/download-2.0.php),
[SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/),
[SDL2_image](https://www.libsdl.org/projects/SDL_image/) and
[cmake](https://cmake.org/).

+ clone this repo
```
git clone https://github.com/BigRedEye/rterm.git
cd rterm
```

+ build
```
cmake . -G "MinGW Makefiles"
mingw32-make
```

+ it's highly recommend to build example programs
```
copy librterm.dll demo/librterm.dll
cd demo
cmake . -G "MinGW Makefiles"
mingw32-make
```

+ to run a test or your program, put librterm.dll and all .dll's from SDL2 archives (SDL2.dll, SDL2_ttf.dll, SDL2_image.dll, libfreetype-6.dll, libpng16-16.dll, libwebp-7.dll, libtiff-5.dll, libjpeg-9.dll, zlib1.dll) next to .exe file.

