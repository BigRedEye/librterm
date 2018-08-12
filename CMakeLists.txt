cmake_minimum_required(VERSION 3.5)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

project(rterm)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -ggdb -fno-omit-frame-pointer -fsanitize=address")

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
    include/virtualconsole.h
    include/window.h
    include/term.tpp
    include/glyph_cache.h
    include/texture_view.h
    include/screen_view.h
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

if (MINGW)
    target_link_libraries(${PROJECT_NAME} -lmingw32 -lSDL2main)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
endif (MINGW)

target_link_libraries(${PROJECT_NAME} -lSDL2 -lSDL2_ttf -lSDL2_image -lpthread)

if (MINGW)
    install(TARGETS ${PROJECT_NAME}
            RUNTIME DESTINATION lib
            PUBLIC_HEADER DESTINATION include/rterm
    )
else ()
    install(TARGETS ${PROJECT_NAME}
            LIBRARY DESTINATION lib
            PUBLIC_HEADER DESTINATION include/rterm
    )
endif (MINGW)
