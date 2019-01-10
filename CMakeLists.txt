cmake_minimum_required(VERSION 3.5)

project(rterm)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    "${PROJECT_SOURCE_DIR}/third_party/sdl2-cmake"
    "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(RTERM_DISABLE_SDL "Diable SDL renderer" OFF)
option(RTERM_DISABLE_OGL "Disable OpenGL renderer" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
set(RTERM_HAS_SDL ON)
set(RTERM_HAS_OGL OFF)

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

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})


if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        COMPILE_FLAGS
        "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic -Wshadow"
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

if(DEFINED RTERM_SDL2_PATH)
    set(SDL2_PATH ${RTERM_SDL2_PATH})
    set(SDL2_TTF_PATH ${RTERM_SDL2_PATH})
endif()

find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# STB_Image
add_subdirectory(third_party/stb)
set(RTERM_INTERNAL_LIBS ${RTERM_INTERNAL_LIBS} stb_image)

# OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL)
if (OPENGL_FOUND)
    set(GLAD_PROFILE "core")
    set(GLAD_API "gl=3.3")
    set(GLAD_GENERATOR "c")
    set(GLAD_SPEC "gl")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad)

    set(RTERM_HAS_OGL ON)
    set(RTERM_INTERNAL_LIBS ${RTERM_INTERNAL_LIBS} OpenGL::GL glad)
    set(RTERM_HEADERS ${RTERM_HEADERS} OPENGL_INCLUDE_DIRS GLAD_INCLUDE_DIRS)
endif(OPENGL_FOUND)

set(RTERM_PUBLIC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    ${SDL2_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
)

set(RTERM_HEADERS
    ${RTERM_PUBLIC_HEADERS}
)

set(RTERM_INTERNAL_LIBS
    ${RTERM_INTERNAL_LIBS}
    ${SDL2_LIBRARY}
    ${SDL2_TTF_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${RTERM_HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

target_link_libraries(${PROJECT_NAME} PUBLIC ${RTERM_INTERNAL_LIBS})

# RTERM_ENABLE_XXX = !RTERM_DISABLE_XXX && RTERM_HAS_XXX
target_compile_definitions(
    ${PROJECT_NAME} PUBLIC
    RTERM_ENABLE_SDL=$<AND:$<NOT:$<BOOL:${RTERM_DISABLE_SDL}>>,$<BOOL:${RTERM_HAS_SDL}$>>
    RTERM_ENABLE_OGL=$<AND:$<NOT:$<BOOL:${RTERM_DISABLE_OGL}>>,$<BOOL:${RTERM_HAS_OGL}$>>
)

set(RTERM_BUILD_TESTS ON)
if (RTERM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif(RTERM_BUILD_TESTS)
