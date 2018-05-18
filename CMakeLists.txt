cmake_minimum_required(VERSION 3.5)

project(rterm)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic")

file(
    GLOB SOURCES
    src/*.cpp
)

file(
    GLOB HEADERS
    include/*.h
    include/*.tpp
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
