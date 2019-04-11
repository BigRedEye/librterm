cmake_minimum_required(VERSION 3.5)

project(rterm)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Modules
list(APPEND CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/third_party/sdl2-cmake-scripts
    ${PROJECT_SOURCE_DIR}/third_party/cmake-conan
    ${PROJECT_SOURCE_DIR}/cmake
)

# Configuration
option(RTERM_DISABLE_SDL "Diable SDL renderer" OFF)
option(RTERM_DISABLE_OGL "Disable OpenGL renderer" OFF)
option(RTERM_BUILD_TESTS "Build tests" OFF)
option(RTERM_SANITIZERS "librterm built with ASan; skip all dlclose calls" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)


set(RTERM_EXTERNAL_LIBS
    "system"
    CACHE STRING
    "Where to get external libraries: from the system, conan or download and build manually")
set_property(CACHE RTERM_EXTERNAL_LIBS PROPERTY STRINGS system conan local)

set(RTERM_HAS_SDL ON)
set(RTERM_HAS_OGL OFF)

# Output binary directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Sanitizers build type
include(Sanitizers)

file(
    GLOB SOURCES
    src/rasterizer/glyph_cache.cpp
    src/rasterizer/tilefont.cpp
    src/rasterizer/ttfont.cpp

    src/renderer/software_texture.cpp

    src/term/term.cpp
    src/term/virtualconsole.cpp

    src/util/char.cpp
    src/util/color.cpp
    src/util/error.cpp
    src/util/framerate_counter.cpp
    src/util/logger.cpp

    src/util/sdl/sdl_loader.cpp

    src/window/event.cpp
    src/window/event_system.cpp
    src/window/key.cpp
    src/window/window.cpp
)

file(
    GLOB HEADERS
    include/rterm/rterm.h

    include/rterm/rasterizer/font.h
    include/rterm/rasterizer/glyph_cache.h
    include/rterm/rasterizer/tilefont.h
    include/rterm/rasterizer/ttfont.h

    include/rterm/renderer/gl_hardware_texture.tpp
    include/rterm/renderer/gl_renderer.tpp
    include/rterm/renderer/hardware_texture.h
    include/rterm/renderer/renderer.h
    include/rterm/renderer/screen_view.h
    include/rterm/renderer/sdl_hardware_texture.tpp
    include/rterm/renderer/sdl_renderer.tpp
    include/rterm/renderer/software_texture.h
    include/rterm/renderer/texture_view.h

    include/rterm/term/term.h
    include/rterm/term/term.tpp
    include/rterm/term/term_format.h
    include/rterm/term/virtualconsole.h

    include/rterm/util/api.h
    include/rterm/util/char.h
    include/rterm/util/color.h
    include/rterm/util/error.h
    include/rterm/util/framerate_counter.h
    include/rterm/util/logger.h
    include/rterm/util/rect.h
    include/rterm/util/sdl/sdl_loader.h
    include/rterm/util/sdl/sdl_ptr.h
    include/rterm/util/span.h
    include/rterm/util/util.h

    include/rterm/window/event.h
    include/rterm/window/event_system.h
    include/rterm/window/key.h
    include/rterm/window/keycode.h
    include/rterm/window/mouse.h
    include/rterm/window/window.h
)

# Main library
add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_library(rterm::rterm ALIAS ${PROJECT_NAME})

# Warnings

if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        COMPILE_FLAGS
        "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wshadow"
    )
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC")
    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        COMPILE_FLAGS
        "${CMAKE_CXX_FLAGS} /W4"
    )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

## Dependencies ##

set(RTERM_PRIVATE_LIBS)
set(RTERM_PRIVATE_HEADERS)
set(RTERM_PUBLIC_LIBS)
set(RTERM_PUBLIC_HEADERS)

list(APPEND RTERM_PUBLIC_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/)

find_package(Threads REQUIRED)
list(APPEND RTERM_PUBLIC_LIBS Threads::Threads)

if (MINGW)
    list(APPEND RTERM_PUBLIC_LIBS -lmingw32)
endif()

if (RTERM_EXTERNAL_LIBS STREQUAL system)
    # Poor windows users
    # Conan is better btw
    if(DEFINED RTERM_SDL2_PATH)
        set(SDL2_PATH ${RTERM_SDL2_PATH})
        set(SDL2_TTF_PATH ${RTERM_SDL2_PATH})
    endif()

    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)

    list(APPEND RTERM_PUBLIC_LIBS ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARIES})
    list(APPEND RTERM_PUBLIC_HEADERS ${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIRS})
elseif (RTERM_EXTERNAL_LIBS STREQUAL conan)
    set(CACHED_BUILD_TYPE ${CMAKE_BUILD_TYPE})
    if (CMAKE_BUILD_TYPE STREQUAL Sanitizers)
        # Temporary enable Debug build type
        set(CMAKE_BUILD_TYPE Debug)
    endif()

    include(conan)
    conan_check(REQUIRED)
    conan_add_remote(NAME bincrafters INDEX 1 URL https://api.bintray.com/conan/bincrafters/public-conan)
    conan_cmake_run(CONANFILE conanfile.py BASIC_SETUP CMAKE_TARGETS BUILD missing)
    set(CMAKE_BUILD_TYPE ${CACHED_BUILD_TYPE})

    list(APPEND RTERM_PUBLIC_LIBS CONAN_PKG::sdl2 CONAN_PKG::sdl2_ttf)
elseif (RTERM_EXTERNAL_LIBS STREQUAL local)
    # We need to find git to patch SDL2 & SDL2_ttf
    find_package(Git REQUIRED)

    include(FetchContent)
    FetchContent_Declare(
        sdl2
        URL https://github.com/SDL-mirror/SDL/archive/release-2.0.9.tar.gz
        PATCH_COMMAND ${GIT_EXECUTABLE} init && ${GIT_EXECUTABLE} apply --ignore-space-change --ignore-whitespace ${CMAKE_CURRENT_LIST_DIR}/third_party/patch/sdl2.patch
    )
    FetchContent_GetProperties(sdl2)
    if(NOT sdl2_POPULATED)
        FetchContent_Populate(sdl2)
        set(SDL_SHARED_ENABLED_BY_DEFAULT OFF)
        add_subdirectory(${sdl2_SOURCE_DIR} ${sdl2_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()

    FetchContent_Declare(
        Freetype
        URL https://download.savannah.gnu.org/releases/freetype/freetype-2.10.0.tar.gz
    )
    FetchContent_GetProperties(Freetype)
    if(NOT Freetype_POPULATED)
        set(CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz TRUE)
        set(CMAKE_DISABLE_FIND_PACKAGE_BZip2 TRUE)
        set(CMAKE_DISABLE_FIND_PACKAGE_ZLIB TRUE)
        set(CMAKE_DISABLE_FIND_PACKAGE_PNG TRUE)
        FetchContent_Populate(Freetype)
        add_subdirectory(${freetype_SOURCE_DIR} ${freetype_BINARY_DIR} EXCLUDE_FROM_ALL)
        add_library(Freetype::Freetype ALIAS freetype)
    endif()

    FetchContent_Declare(
        sdl2-ttf
        URL https://github.com/SDL-mirror/SDL_ttf/archive/release-2.0.15.tar.gz
        PATCH_COMMAND ${GIT_EXECUTABLE} init && ${GIT_EXECUTABLE} apply --ignore-space-change --ignore-whitespace ${CMAKE_CURRENT_LIST_DIR}/third_party/patch/sdl2-ttf.patch
    )
    FetchContent_GetProperties(sdl2-ttf)

    if(NOT sdl2-ttf_POPULATED)
        FetchContent_Populate(sdl2-ttf)
        set(SDL_SHARED_ENABLED_BY_DEFAULT OFF)
        add_subdirectory(${sdl2-ttf_SOURCE_DIR} ${sdl2-ttf_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
    add_library(SDL2::SDL2 ALIAS SDL2-static)

    list(APPEND RTERM_PUBLIC_LIBS freetype SDL2main SDL2-static SDL2_ttf)
    list(APPEND RTERM_PUBLIC_HEADERS ${sdl2-ttf_SOURCE_DIR})
else()
    message(FATAL_ERROR
        "Invalid RTERM_EXTERNAL_LIBS value: ${RTERM_EXTERNAL_LIBS}, should be one 'system', 'conan' or 'local'")
endif()

# STB_Image
add_subdirectory(third_party/stb)
list(APPEND RTERM_PRIVATE_LIBS stb_image)

# utfcpp
set(UTF8_TESTS OFF CACHE BOOL "" FORCE)
set(UTF8_SAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/utfcpp EXCLUDE_FROM_ALL)
list(APPEND RTERM_PRIVATE_LIBS utf8::cpp)

# OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL)
if (OPENGL_FOUND)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad)

    set(RTERM_HAS_OGL ON)
    list(APPEND RTERM_PRIVATE_LIBS glad)
    list(APPEND RTERM_PUBLIC_LIBS OpenGL::GL)
    list(APPEND RTERM_PRIVATE_HEADERS OPENGL_INCLUDE_DIRS)
endif(OPENGL_FOUND)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${RTERM_PRIVATE_HEADERS}
    PUBLIC ${RTERM_PUBLIC_HEADERS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${RTERM_PRIVATE_LIBS}
    PUBLIC ${RTERM_PUBLIC_LIBS}
)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

# RTERM_ENABLE_XXX = !RTERM_DISABLE_XXX && RTERM_HAS_XXX
target_compile_definitions(
    ${PROJECT_NAME} PUBLIC
    RTERM_ENABLE_SDL=$<AND:$<NOT:$<BOOL:${RTERM_DISABLE_SDL}>>,$<BOOL:${RTERM_HAS_SDL}$>>
    RTERM_ENABLE_OGL=$<AND:$<NOT:$<BOOL:${RTERM_DISABLE_OGL}>>,$<BOOL:${RTERM_HAS_OGL}$>>
    RTERM_SANITIZERS=$<BOOL:${RTERM_SANITIZERS}>
)

if (RTERM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif(RTERM_BUILD_TESTS)
