cmake_minimum_required(VERSION 3.5)

set(download_project_dir ${CMAKE_CURRENT_LIST_DIR})

function(download_project project)
    set(config_file "${download_project_dir}/External${project}.txt.in")
    configure_file(${config_file} ${project}-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${project}-download )
    if(result)
        message(FATAL_ERROR "CMake step for ${project} failed: ${result}")
    endif()

    set(PARALLEL_ARG)
    set(PARALLEL_CORES)
    # cmake --build --parallel was introduced in 3.12
    if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.12.0")
        include(ProcessorCount)
        ProcessorCount(NPROC)
        if (NOT NPROC EQUAL 0)
            set(PARALLEL_ARG "--parallel")
            set(PARALLEL_CORES ${NPROC})
        endif()
    endif()

    execute_process(COMMAND ${CMAKE_COMMAND} --build . ${PARALLEL_ARG} ${PARALLEL_CORES}
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${project}-download )
    if(result)
        message(FATAL_ERROR "Build step for ${project} failed: ${result}")
    endif()

    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/${project}-src
                     ${CMAKE_CURRENT_BINARY_DIR}/${project}-build
                     EXCLUDE_FROM_ALL)
endfunction()
