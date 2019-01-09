cmake_minimum_required(VERSION 3.5)

project(stb_image)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB HEADERS
    stb/stb_image.h
)

file(GLOB SOURCES
    src/stb_image.cpp
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stb/)
