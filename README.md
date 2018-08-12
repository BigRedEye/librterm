# rterm #

[![Build Status](https://travis-ci.org/BigRedEye/rterm.svg)](https://travis-ci.org/BigRedEye/rterm)
[![Build status](https://ci.appveyor.com/api/projects/status/ch37wqe58bkt6577?svg=true)](https://ci.appveyor.com/project/BigRedEye/rterm)
[![GitHub tag](https://img.shields.io/github/tag/BigRedEye/rterm.svg)](https://semver.org)
[![license](https://img.shields.io/github/license/BigRedEye/rterm.svg)](https://github.com/BigRedEye/rterm/blob/master/LICENSE)

Minimalistic terminal emulator written on C++ and SDL2.

## Minimal example ##
This piece of code creates an empty `80 x 24` terminal and displays each character pressed by the user.
```cpp
#include <rterm/term.h>

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
    } catch (const rterm::Exception& e)
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

```

## Requirements ##

+ Compiler with C++11 suppot
+ [SDL2](https://www.libsdl.org/download-2.0.php)
+ [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)
+ [SDL2_image](https://www.libsdl.org/projects/SDL_image/)
+ [cmake](https://cmake.org/)

## Compiling ##

+ Install the latest version of [SDL2](https://www.libsdl.org/download-2.0.php),
[SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/),
[SDL2_image](https://www.libsdl.org/projects/SDL_image/) and
[cmake](https://cmake.org/).

+ Clone this repository
```sh
git clone https://github.com/BigRedEye/rterm.git
cd rterm
```

### Linux ###

+ Build
```sh
cmake .
make
```

+ Install the library
```sh
sudo make install
```

+ Build examples
```sh
cd test
cmake .
make
```

### Windows (MinGW) ###

+ Put all SDL2 related .dll's anywhere where Windows can find them.

+ Build
```
cmake . -G "MinGW Makefiles"
mingw32-make
```

+ Alternatively, you can use automatically generated [builds](https://github.com/BigRedEye/rterm/releases).

+ Build examples
```
copy librterm.dll test/librterm.dll
cd test
cmake . -G "MinGW Makefiles"
mingw32-make
```

