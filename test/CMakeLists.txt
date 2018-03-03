cmake_minimum_required(VERSION 3.5)

project(tests)

set(CMAKE_CXX_STANDARD 11)

file(GLOB sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

foreach(abssourcefile ${sources})
    get_filename_component(sourcefile ${abssourcefile} NAME)
    
    string(REPLACE ".cpp" "" exename ${sourcefile})
    add_executable(${exename} ${sourcefile})
    target_link_libraries(${exename} -lrterm)

endforeach(abssourcefile ${sources})
