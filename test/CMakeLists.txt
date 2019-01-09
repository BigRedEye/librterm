cmake_minimum_required(VERSION 3.5)

project(tests)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE resources "${PROJECT_SOURCE_DIR}/../resources/*")

foreach(abssourcefile ${sources})
    get_filename_component(sourcefile ${abssourcefile} NAME)
    
    string(REPLACE ".cpp" "" exename ${sourcefile})
    file(COPY ${resources} DESTINATION resources)
    add_executable(${exename} ${sourcefile} ${resources})
    target_link_libraries(${exename} rterm)

endforeach(abssourcefile ${sources})
