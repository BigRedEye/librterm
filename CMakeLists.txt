cmake_minimum_required(VERSION 3.5)

project(rterm)

# Modules
list(APPEND CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/third_party/cmake-conan
    ${PROJECT_SOURCE_DIR}/cmake
)

# Configuration
option(RTERM_BUILD_TESTS "Build tests" OFF)
option(RTERM_DISABLE_VULKAN "Disable Vulkan" OFF)
option(RTERM_DISABLE_OPENGL "Disable OpenGL" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(RTERM_HAS_OPENGL OFF)
set(RTERM_HAS_VULKAN OFF)

set(RTERM_EXTERNAL_LIBS
    system
    CACHE STRING
    "Where to get external libraries: from the system, conan or setup cmake targets manually")
set_property(CACHE RTERM_EXTERNAL_LIBS PROPERTY STRINGS system conan manual)

# Output binary directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Sanitizers build type
include(Sanitizers)

file(
    GLOB SOURCES
    src/terminal/terminal.cpp
)

file(
    GLOB HEADERS
)

# Main library
add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_library(rterm::rterm ALIAS ${PROJECT_NAME})

# Warnings
if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        COMPILE_FLAGS
        "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wshadow"
    )
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC")
    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        COMPILE_FLAGS
        "${CMAKE_CXX_FLAGS} /W4"
    )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

## Dependencies ##
set(RTERM_PRIVATE_LIBS)
set(RTERM_PRIVATE_HEADERS)
set(RTERM_PUBLIC_LIBS)
set(RTERM_PUBLIC_HEADERS)

list(APPEND RTERM_PUBLIC_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/)

find_package(Threads REQUIRED)
list(APPEND RTERM_PUBLIC_LIBS Threads::Threads)

if (MINGW)
    list(APPEND RTERM_PUBLIC_LIBS -lmingw32)
endif()

if (RTERM_EXTERNAL_LIBS STREQUAL system)
    find_package(ICU COMPONENTS data uc REQUIRED)
    find_package(Freetype REQUIRED)
    find_package(Harfbuzz REQUIRED)
    list(APPEND RTERM_PRIVATE_LIBS ICU::data ICU::uc Freetype::Freetype harfbuzz)
elseif (RTERM_EXTERNAL_LIBS STREQUAL conan)
    set(CACHED_BUILD_TYPE ${CMAKE_BUILD_TYPE})
    if (CMAKE_BUILD_TYPE STREQUAL Sanitizers)
        # Temporary enable Debug build type
        set(CMAKE_BUILD_TYPE Debug)
    endif()

    include(conan)
    conan_check(REQUIRED)
    conan_add_remote(NAME bincrafters INDEX 1 URL https://api.bintray.com/conan/bincrafters/public-conan)
    conan_cmake_run(CONANFILE conanfile.py BASIC_SETUP CMAKE_TARGETS BUILD missing)
    set(CMAKE_BUILD_TYPE ${CACHED_BUILD_TYPE})

    list(APPEND RTERM_PUBLIC_LIBS CONAN_PKG::icu CONAN_PKG::freetype CONAN_PKG::harfbuzz)
elseif(RTERM_EXTERNAL_LIBS STREQUAL manual)
    list(APPEND RTERM_PUBLIC_LIBS ${RTERM_MANUAL_LIBS})
    list(APPEND RTERM_PUBLIC_HEADERS ${RTERM_MANUAL_HEADERS})
else()
    message(FATAL_ERROR
        "Invalid RTERM_EXTERNAL_LIBS value: ${RTERM_EXTERNAL_LIBS}, should be one of 'system', 'conan' or 'manual'")
endif()

# glfw
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glfw EXCLUDE_FROM_ALL)
list(APPEND RTERM_PRIVATE_LIBS glfw)

# STB_Image
add_subdirectory(third_party/stb)
list(APPEND RTERM_PRIVATE_LIBS stb_image)

# OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL)
if (OPENGL_FOUND)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad)

    set(RTERM_HAS_OPENGL ON)
    list(APPEND RTERM_PRIVATE_LIBS glad)
    list(APPEND RTERM_PUBLIC_LIBS OpenGL::GL)
    list(APPEND RTERM_PRIVATE_HEADERS OPENGL_INCLUDE_DIRS)
endif(OPENGL_FOUND)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${RTERM_PRIVATE_HEADERS}
    PUBLIC ${RTERM_PUBLIC_HEADERS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${RTERM_PRIVATE_LIBS}
    PUBLIC ${RTERM_PUBLIC_LIBS}
)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

# RTERM_ENABLE_XXX = !RTERM_DISABLE_XXX && RTERM_HAS_XXX
target_compile_definitions(
    ${PROJECT_NAME} PUBLIC
    RTERM_ENABLE_OPENGL=$<AND:$<NOT:$<BOOL:${RTERM_DISABLE_OPENGL}>>,$<BOOL:${RTERM_HAS_OPENGL}$>>
    RTERM_ENABLE_VULKAN=$<AND:$<NOT:$<BOOL:${RTERM_DISABLE_VULKAN}>>,$<BOOL:${RTERM_HAS_VULKAN}$>>
)

if (RTERM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif(RTERM_BUILD_TESTS)
