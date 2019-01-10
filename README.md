# rterm #

[![Build Status](https://travis-ci.org/BigRedEye/rterm.svg?branch=master)](https://travis-ci.org/BigRedEye/rterm)
[![Build status](https://ci.appveyor.com/api/projects/status/ch37wqe58bkt6577/branch/master?svg=true
)](https://ci.appveyor.com/project/BigRedEye/rterm)
[![GitHub tag](https://img.shields.io/github/tag/BigRedEye/rterm.svg)](https://semver.org)
[![license](https://img.shields.io/github/license/BigRedEye/rterm.svg)](https://github.com/BigRedEye/rterm/blob/master/LICENSE)

Minimalistic terminal emulator written on C++ and SDL2.

## Minimal example ##
This piece of code creates an empty `80 x 24` terminal and displays each character pressed by the user.
```cpp
#include <rterm/rterm.h>

int main(int argc, const char* argv[]) {
    try {
        rterm::Term terminal(rterm::TermFormat(80, 24));
        terminal.setFont("DejaVuSansMono.ttf", 18);
        terminal.onKeyDown([&](const rterm::events::KeyDownEvent &ev) {
            if (ev.key().toChar()) {
                terminal.addChar(ev.key().toChar());
            }
            terminal.redraw();
        });
        while (terminal.isRunning()) {
            terminal.poll();
        }
        return 0;
    } catch (const rterm::Exception& e) {
        rterm::Logger() << e.what();
        return 1;
    }
}

```

## Requirements ##

+ Compiler with C++14 suppot
+ [SDL2](https://www.libsdl.org/download-2.0.php)
+ [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)
+ [cmake](https://cmake.org/)

## Compiling ##

+ Install the latest version of [SDL2](https://www.libsdl.org/download-2.0.php),
[SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/) and
[cmake](https://cmake.org/).

+ Clone this repository
```sh
git clone --recurse-submodules https://github.com/BigRedEye/rterm.git
cd rterm
```

### Linux ###

+ Build
```sh
mkdir build
cd build

cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel $(nproc)
```

### Windows ###

+ Download latest [SDL2](https://www.libsdl.org/download-2.0.php) and [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/) development libraries.

+ Extract the binaries for your architecture and compiler and place them somewhere, for example, in the `%SDL2_LIBS%` directory

+ Add `%SDL2_LIBS%\bin` to PATH

+ Create build directory
```
mkdir build
cd build
```

###### MinGW
```
cmake .. -DCMAKE_BUILD_TYPE=Release -DRTERM_SDL2_PATH=%SDL2_LIBS% -G "MinGW Makefiles"
cmake --build . --parallel 8
```

###### MSVC
```
cmake .. -DCMAKE_BUILD_TYPE=Release -DRTERM_SDL2_PATH=%SDL2_LIBS% -G "Visual Studio 15 2017" -A x64
cmake --build . --parallel 8
```

+ Alternatively, you can use automatically generated [builds](https://github.com/BigRedEye/rterm/releases).
