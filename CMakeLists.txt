cmake_minimum_required(VERSION 3.5)

project(rterm)

set (CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES
    src/*.cpp
)

file(GLOB HEADERS
    include/*.h
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

if (WIN32)
    target_link_libraries(${PROJECT_NAME} -lmingw32 -lSDL2main)
endif (WIN32)

target_link_libraries(${PROJECT_NAME} -lSDL2 -lSDL2_ttf -lSDL2_image -lpthread)

if (WIN32)
    install(TARGETS ${PROJECT_NAME}
            RUNTIME DESTINATION lib
            PUBLIC_HEADER DESTINATION include/rterm
    )
else ()
    install(TARGETS ${PROJECT_NAME}
            LIBRARY DESTINATION lib
            PUBLIC_HEADER DESTINATION include/rterm
    )
endif (WIN32)
