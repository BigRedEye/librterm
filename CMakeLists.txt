cmake_minimum_required(VERSION 2.8)

project(libterm)
file(GLOB SOURCES
    src/*.h
    src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} -lmingw32)
target_link_libraries(${PROJECT_NAME} -lSDL2main)
target_link_libraries(${PROJECT_NAME} -lSDL2)
target_link_libraries(${PROJECT_NAME} -lSDL2_ttf)
target_link_libraries(${PROJECT_NAME} -mwindows)
