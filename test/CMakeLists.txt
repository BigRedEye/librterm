cmake_minimum_required(VERSION 3.5)

project(tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE resources "${PROJECT_SOURCE_DIR}/../resources/*")

if(POLICY CMP0069)
    cmake_policy(SET CMP0069 NEW)
endif()

foreach(abssourcefile ${sources})
    get_filename_component(sourcefile ${abssourcefile} NAME)

    string(REPLACE ".cpp" "" exename ${sourcefile})
    file(COPY ${resources} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)
    add_executable(${exename} ${sourcefile} ${resources})
    target_link_libraries(${exename} rterm::rterm)
    if(CMAKE_VERSION VERSION_LESS "3.9.0")
        message(STATUS "Old cmake version (${CMAKE_VERSION}); LTO is not supported.")
    elseif(MINGW)
        message(STATUS "MinGW does not support LTO")
    else()
        include(CheckIPOSupported)
        check_ipo_supported(RESULT result OUTPUT output)
        if(result)
            set_property(TARGET ${exename} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
        else()
            message(WARNING "LTO is not supported: ${output}")
        endif()
    endif()

endforeach(abssourcefile ${sources})
