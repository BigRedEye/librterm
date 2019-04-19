# librterm

[![Build Status](https://travis-ci.org/BigRedEye/librterm.svg?branch=dev)](https://travis-ci.org/BigRedEye/librterm)
[![Build status](https://ci.appveyor.com/api/projects/status/ch37wqe58bkt6577/branch/dev?svg=true)](https://ci.appveyor.com/project/BigRedEye/librterm/branch/dev)
[![GitHub tag](https://img.shields.io/github/tag/BigRedEye/librterm.svg)](https://semver.org)
[![license](https://img.shields.io/github/license/BigRedEye/librterm.svg?color=blue)](https://github.com/BigRedEye/librterm/blob/dev/LICENSE)

Terminal emulator library.

## Motivating example
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

## Requirements
+ Compiler with C++14 support (Clang 7, GCC 8, MSVC 19.16, and MinGW-w64 7.2 tested)
+ [cmake](https://cmake.org/)

## Installation ##
Add the library as a git submodule:
```sh
$ git submodule add -b master https://github.com/BigRedEye/librterm.git third_party/librterm
$ git submodule update --init --recursive
```
And include into your build tree:
```cmake
set(RTERM_EXTERNAL_LIBS ...)
add_subdirectory(third_party/librterm)
...
target_link_libraries(${YOUR_EXECUTABLE} PRIVATE rterm::rterm)
```
The `RTERM_EXTERNAL_LIBS` option is documented in the [dependencies](#dependencies) section.

## Dependencies
librterm requires some external dependecies ([SDL2](https://www.libsdl.org/download-2.0.php) and [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)). The library supports a few different methods of dependency management:
+ [conan (preferred)](#conan)
+ [system libraries (enabled by default)](#system)
+ [automatically downloaded libraries](#local)
+ [manual](#manual)

To choose one, set option `RTERM_EXTERNAL_LIBS=(conan|system|local|manual)` in your CMakeLists.txt.
#### Conan
The easiest way to obtain them is to use [conan](https://conan.io). Just install it:
```sh
$ pip install conan
```
And set `RTERM_EXTERNAL_LIBS`:
```cmake
set(RTERM_EXTERNAL_LIBS conan)
```

#### System
###### Linux
+ Install sdl2 and sdl2-ttf development packages:
```sh
# Arch Linux:
$ sudo pacman -S sdl2 sdl2-ttf

# Ubuntu:
$ sudo apt-get install libsdl2-dev libsdl2-ttf-dev
```
+ Set `RTERM_EXTERNAL_LIBS` to `system`:
```cmake
set(RTERM_EXTERNAL_LIBS system)
```
###### Windows
+ Download [SDL2](https://www.libsdl.org/download-2.0.php) and
[SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/) development libraries (...-VS.zip for MSVC, ...-mingw.tar.gz for MinGW-w64).
+ Extract downloaded archives somewhere, for example to `%SDL_LIBS%`.
+ Add `%SDL_LIBS%` to `PATH`
+ Configure cmake:
```cmake
set(RTERM_SDL2_PATH %SDL_LIBS%)
set(RTERM_EXTERNAL_LIBS system)
```
Alternatively you can pass `%SDL_LIBS%` directly to cmake:
```batch
cmake ... -RTERM_SDL2_PATH=%SDL_LIBS%
```

Really, use conan.

#### Local
In this mode librterm will try to download and build required libraries automatically. This increases build time while provides better debugging / profiling results. Set `RTERM_EXTERNAL_LIBS` to `local`:
```cmake
set(RTERM_EXTERNAL_LIBS local)
```

#### Manual
If you are brave enough, you can try to setup cmake targets for SDL2, SDL2main and SDL2_ttf manually:
+ Set `RTERM_MANUAL_LIBS` as a list of required targets and optionally set `RTERM_MANUAL_HEADERS`. For example:
```cmake
set(RTERM_MANUAL_LIBS)
set(RTERM_MANUAL_HEADERS)
list(APPEND RTERM_MANUAL_LIBS SDL2)
list(APPEND RTERM_MANUAL_LIBS SDL2_ttf)
list(APPEND RTERM_MANUAL_HEADERS ${SDL2_HEADERS})
```
+ Set `RTERM_EXTERNAL_LIBS` to `manual`:
```cmake
set(RTERM_EXTERNAL_LIBS manual)
```
librterm tested with SDL2 2.0.9 and SDL2_ttf 2.0.15.
