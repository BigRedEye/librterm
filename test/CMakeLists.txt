cmake_minimum_required(VERSION 3.5)

project(tests)

set(CMAKE_CXX_STANDARD 11)

file(GLOB sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

foreach(abssourcefile ${sources})
    get_filename_component(sourcefile ${abssourcefile} NAME)
    
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

    string(REPLACE ".cpp" "" exename ${sourcefile})
    add_executable(${exename} ${sourcefile})

    if (WIN32)
        target_link_libraries(${exename} -lmingw32)
        target_link_libraries(${exename} -lSDL2main)
    endif (WIN32)

    target_link_libraries(${exename} -lSDL2)
    target_link_libraries(${exename} -lSDL2_ttf)
    target_link_libraries(${exename} -lSDL2_image)
    target_link_libraries(${exename} -lrterm)

    if (WIN32)
        target_link_libraries(${exename} -mwindows)
    endif (WIN32)

endforeach(abssourcefile ${sources})
