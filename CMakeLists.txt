cmake_minimum_required(VERSION 3.5)

project(rterm)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/third_party/sdl2-cmake")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (${CMAKE_CXX_COMPILER_ID} MATCHES  "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic")
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES  "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic")
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES  "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

option(RTERM_DISABLE_SDL "Diable SDL renderer" OFF)
option(RTERM_DISABLE_OGL "Disable OpenGL renderer" OFF)
set(RTERM_HAS_SDL ON)
set(RTERM_HAS_OGL OFF)

file(
    GLOB SOURCES
    src/char.cpp
    src/color.cpp
    src/error.cpp
    src/event.cpp
    src/event_system.cpp
    src/framerate_counter.cpp
    src/glyph_cache.cpp
    src/key.cpp
    src/logger.cpp
    src/sdl_loader.cpp
    src/software_texture.cpp
    src/term.cpp
    src/texture.cpp
    src/tilefont.cpp
    src/ttfont.cpp
    src/virtualconsole.cpp
    src/window.cpp
)

file(
    GLOB HEADERS
    include/rterm/api.h
    include/rterm/char.h
    include/rterm/color.h
    include/rterm/error.h
    include/rterm/event.h
    include/rterm/event_system.h
    include/rterm/font.h
    include/rterm/framerate_counter.h
    include/rterm/hardware_texture.h
    include/rterm/key.h
    include/rterm/keycode.h
    include/rterm/logger.h
    include/rterm/mouse.h
    include/rterm/rect.h
    include/rterm/renderer.h
    include/rterm/gl_hardware_texture.tpp
    include/rterm/gl_renderer.tpp
    include/rterm/sdl_hardware_texture.tpp
    include/rterm/sdl_renderer.tpp
    include/rterm/sdl_loader.h
    include/rterm/sdl_ptr.h
    include/rterm/software_texture.h
    include/rterm/term.h
    include/rterm/term_format.h
    include/rterm/texture.h
    include/rterm/tilefont.h
    include/rterm/ttfont.h
    include/rterm/util.h
    include/rterm/virtualconsole.h
    include/rterm/window.h
    include/rterm/term.tpp
    include/rterm/glyph_cache.h
    include/rterm/texture_view.h
    include/rterm/screen_view.h
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

if (MINGW)
    set(SDL2_PATH C:/Tools/Libs/mingw/x64/SDL2/x86_64-w64-mingw32)
    set(SDL2_IMAGE_PATH C:/Tools/Libs/mingw/x64/SDL2_image/x86_64-w64-mingw32)
    set(SDL2_TTF_PATH C:/Tools/Libs/mingw/x64/SDL2_ttf/x86_64-w64-mingw32)
endif()

set(RTERM_HEADERS)
set(RTERM_INTERNAL_LIBS)

find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

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

set(RTERM_HEADERS
   ${RTERM_HEADERS}
   ${CMAKE_CURRENT_SOURCE_DIR}/include/
   ${SDL2_INCLUDE_DIR}
   ${SDL2_IMAGE_INCLUDE_DIR}
   ${SDL2_TTF_INCLUDE_DIR}
)

set (RTERM_INTERNAL_LIBS
    ${RTERM_INTERNAL_LIBS}
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${RTERM_HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

target_link_libraries(${PROJECT_NAME} PUBLIC ${RTERM_INTERNAL_LIBS})

# RTERM_HAS_XXX = !RTERM_DISABLE_XXX && RTERM_HAS_XXX
target_compile_definitions(
    ${PROJECT_NAME} PUBLIC
    RTERM_HAS_SDL=$<AND:$<NOT:$<BOOL:${RTERM_DISABLE_SDL}>>,$<BOOL:${RTERM_HAS_SDL}$>>
    RTERM_HAS_OGL=$<AND:$<NOT:$<BOOL:${RTERM_DISABLE_OGL}>>,$<BOOL:${RTERM_HAS_OGL}$>>
)

set(RTERM_ADD_TEST_TARGET ON)
if (RTERM_ADD_TEST_TARGET)
    add_subdirectory(test)
endif(RTERM_ADD_TEST_TARGET)
