# rterm #

[![Build Status](https://travis-ci.com/BigRedEye/rterm.svg?token=HabA2F1p73cnpyrz3Jdj&branch=master)](https://travis-ci.com/BigRedEye/rterm)
[![GitHub tag](https://img.shields.io/github/tag/BigRedEye/rterm.svg)](https://semver.org)
[![license](https://img.shields.io/github/license/BigRedEye/rterm.svg)](https://github.com/BigRedEye/rterm/blob/master/LICENSE)

Minimalistic terminal emulator written on C++ and SDL2.
    
## Requirements ##

+ Compiler that supports C++11
+ [SDL2](https://www.libsdl.org/download-2.0.php)
+ [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)
+ [SDL2_image](https://www.libsdl.org/projects/SDL_image/)
+ [cmake](https://cmake.org/)

## Compiling ##

+ Install the latest version of [SDL2](https://www.libsdl.org/download-2.0.php),
[SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/),
[SDL2_image](https://www.libsdl.org/projects/SDL_image/) and
[cmake](https://cmake.org/).

+ Clone this repo
```
git clone https://github.com/BigRedEye/rterm.git
cd rterm
```

### Linux ###

+ Build
```
cmake .
make
```

+ Install the library
```
sudo make install
```

+ Build examples:
```
cd test
cmake .
make
```

### Windows (MinGW) ###

+ Build:
```
cmake . -G "MinGW Makefiles"
mingw32-make
```

+ Build examples:
```
copy librterm.dll demo/librterm.dll
cd demo
cmake . -G "MinGW Makefiles"
mingw32-make
```

+ To run examples or your program, put librterm.dll and all SDL2 related .dll's anywhere where Windows can find them.

