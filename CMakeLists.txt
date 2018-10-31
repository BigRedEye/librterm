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
    include/api.h
    include/char.h
    include/color.h
    include/error.h
    include/event.h
    include/event_system.h
    include/font.h
    include/framerate_counter.h
    include/hardware_texture.h
    include/key.h
    include/keycode.h
    include/logger.h
    include/mouse.h
    include/rect.h
    include/renderer.h
    include/gl_hardware_texture.tpp
    include/gl_renderer.tpp
    include/sdl_hardware_texture.tpp
    include/sdl_renderer.tpp
    include/sdl_loader.h
    include/sdl_ptr.h
    include/software_texture.h
    include/term.h
    include/term_format.h
    include/texture.h
    include/tilefont.h
    include/ttfont.h
    include/util.h
    include/virtualconsole.h
    include/window.h
    include/term.tpp
    include/glyph_cache.h
    include/texture_view.h
    include/screen_view.h
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

set (RTERM_HEADERS
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

target_link_libraries(${PROJECT_NAME} ${RTERM_INTERNAL_LIBS})

# RTERM_HAS_XXX = !RTERM_DISABLE_XXX && RTERM_HAS_XXX
target_compile_definitions(
    ${PROJECT_NAME} PRIVATE
    RTERM_HAS_SDL=$<AND:$<NOT:$<BOOL:${RTERM_DISABLE_SDL}>>,$<BOOL:${RTERM_HAS_SDL}$>>
    RTERM_HAS_OGL=$<AND:$<NOT:$<BOOL:${RTERM_DISABLE_OGL}>>,$<BOOL:${RTERM_HAS_OGL}$>>
)

set(RTERM_LIBS ${RTERM_INTERNAL_LIBS} ${PROJECT_NAME})

set(RTERM_ADD_TEST_TARGET ON)
if (RTERM_ADD_TEST_TARGET)
    add_subdirectory(test)
endif(RTERM_ADD_TEST_TARGET)
