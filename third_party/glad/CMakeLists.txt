cmake_minimum_required(VERSION 3.5)

project(glad)

file(GLOB sources
    src/glad.c
)

file(GLOB headers
    include/glad.h
    include/KHR/khrplatform.h
)

add_library(${PROJECT_NAME} STATIC ${sources} ${headers})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
