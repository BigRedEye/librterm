cmake_minimum_required(VERSION 3.5)

project(rterm)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/third/sdl2-cmake")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (${CMAKE_CXX_COMPILER_ID} MATCHES  "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Werror")
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES  "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic")
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES  "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

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

if (MINGW)
    set(SDL2_PATH C:/Tools/Libs/mingw/x64/SDL2/x86_64-w64-mingw32)
    set(SDL2_IMAGE_PATH C:/Tools/Libs/mingw/x64/SDL2_image/x86_64-w64-mingw32)
    set(SDL2_TTF_PATH C:/Tools/Libs/mingw/x64/SDL2_ttf/x86_64-w64-mingw32)
endif()

find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

set (RTERM_HEADERS
   ${CMAKE_CURRENT_SOURCE_DIR}/include/
   ${SDL2_INCLUDE_DIR}
   ${SDL2_IMAGE_INCLUDE_DIR}
   ${SDL2_TTF_INCLUDE_DIR}
)

set (RTERM_INTERNAL_LIBS
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${RTERM_HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

target_link_libraries(${PROJECT_NAME} ${RTERM_INTERNAL_LIBS})

if (MINGW)
#    install(
#        TARGETS ${PROJECT_NAME}
#        RUNTIME DESTINATION lib
#        PUBLIC_HEADER DESTINATION include/rterm
#    )
else ()
#    install(
#       TARGETS ${PROJECT_NAME}
#        LIBRARY DESTINATION lib
#        PUBLIC_HEADER DESTINATION include/rterm
#    )
endif (MINGW)

set(RTERM_LIBS ${RTERM_INTERNAL_LIBS} ${PROJECT_NAME})

set (ADD_TEST_TARGET True)
if (ADD_TEST_TARGET)
    add_subdirectory(test)
endif(ADD_TEST_TARGET)

export(PACKAGE ${PROJECT_NAME})
