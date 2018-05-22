cmake_minimum_required(VERSION 3.5)

project(rterm)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic")

file(
    GLOB SOURCES
    src/char.cpp
    src/color.cpp
    src/event.cpp
    src/event_system.cpp
    src/framerate_counter.cpp
    src/gl_renderer.cpp
    src/key.cpp
    src/logger.cpp
    src/sdl_loader.cpp
    src/sdl_renderer.cpp
    src/term.cpp
    src/texture.cpp
    src/tilefont.cpp
    src/ttfont.cpp
    src/virtualconsole.cpp
    src/window.cpp
)

file(
    GLOB HEADERS
    include/char.h
    include/color.h
    include/event.h
    include/event_system.h
    include/font.h
    include/framerate_counter.h
    include/key.h
    include/keycode.h
    include/logger.h
    include/mouse.h
    include/rect.h
    include/renderer.h
    include/sdl_loader.h
    include/sdl_ptr.h
    include/term.h
    include/term_format.h
    include/texture.h
    include/tilefont.h
    include/ttfont.h
    include/virtualconsole.h
    include/window.h
    include/term.tpp
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

add_subdirectory(test)

add_custom_command(
    TARGET rterm POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/fonts/ttf/DejaVuSansMono.ttf
            ${CMAKE_CURRENT_BINARY_DIR}/test/DejaVuSansMono.ttf
)

if (WIN32)
    add_custom_command(
        TARGET rterm POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/librterm.dll
                ${CMAKE_CURRENT_BINARY_DIR}/test/librterm.dll
    )
endif (WIN32)
