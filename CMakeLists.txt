cmake_minimum_required(VERSION 2.8)

project(libterm)
file(GLOB SOURCES
    include/*.h
    src/*.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${SOURCES})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} -lmingw32)
    target_link_libraries(${PROJECT_NAME} -lSDL2main)
endif (WIN32)

target_link_libraries(${PROJECT_NAME} -lSDL2)
target_link_libraries(${PROJECT_NAME} -lSDL2_ttf)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} -mwindows)
endif (WIN32)
