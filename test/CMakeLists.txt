cmake_minimum_required(VERSION 3.5)

project(tests)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
endif (MINGW)

file(GLOB sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

foreach(abssourcefile ${sources})
    get_filename_component(sourcefile ${abssourcefile} NAME)
    
    string(REPLACE ".cpp" "" exename ${sourcefile})
    add_executable(${exename} ${sourcefile})
    if (MINGW)
        target_link_libraries(${exename} -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf)
    endif (MINGW)
    target_link_libraries(${exename} -lrterm)

endforeach(abssourcefile ${sources})
