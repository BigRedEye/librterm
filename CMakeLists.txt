cmake_minimum_required(VERSION 3.5)

project(rterm)

file(GLOB SOURCES
    include/*.h
    src/*.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/term.h)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")

if (WIN32)
    target_link_libraries(${PROJECT_NAME} -lmingw32)
    target_link_libraries(${PROJECT_NAME} -lSDL2main)
endif (WIN32)

target_link_libraries(${PROJECT_NAME} -lSDL2)
target_link_libraries(${PROJECT_NAME} -lSDL2_ttf)
target_link_libraries(${PROJECT_NAME} -lSDL2_image)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} -mwindows)
endif (WIN32)
