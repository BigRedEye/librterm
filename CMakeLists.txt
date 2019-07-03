cmake_minimum_required(VERSION 3.5)

project(rterm)

# Modules
list(APPEND CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/third_party/cmake-conan
    ${PROJECT_SOURCE_DIR}/cmake
)

# Detect platform
set(RTERM_OS "RTERM_OS_UNDEFINED")
if (WIN32 OR WIN64)
    set(RTERM_OS "RTERM_OS_WINDOWS")
    set(RTERM_OS_WINDOWS ON)
elseif (APPLE)
    set(RTERM_OS "RTERM_OS_MACOS")
    set(RTERM_OS_MACOS ON)
elseif (UNIX)
    set(RTERM_OS "RTERM_OS_UNIX")
    set(RTERM_OS_UNIX ON)
endif()

# Configuration
option(RTERM_BUILD_TESTS "Build tests" OFF)
option(RTERM_CONFIG_GAPI_VULKAN "Enable Vulkan" ON)
option(RTERM_CONFIG_GAPI_OPENGL "Enable OpenGL" ON)
include(CMakeDependentOption)
CMAKE_DEPENDENT_OPTION(RTERM_CONFIG_SOURCE_FONTCONFIG "Use FontConfig as system font source" ON "RTERM_OS_UNIX" OFF)
CMAKE_DEPENDENT_OPTION(RTERM_CONFIG_SOURCE_CORETEXT "Use CoreText as system font source" ON "RTERM_OS_MACOS" OFF)
CMAKE_DEPENDENT_OPTION(RTERM_CONFIG_SOURCE_DIRECTWRITE "Use DirectWrite as system font source" ON "RTERM_OS_WINDOWS" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

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
    src/util/exception.cpp

    src/terminal/terminal.cpp
    src/terminal/options.cpp
)

file(
    GLOB HEADERS
    include/rterm/rterm.h

    include/rterm/util/api.h
    include/rterm/util/defs.h
    include/rterm/util/enum.h
    include/rterm/util/exception.h
    include/rterm/util/murmur.h
    include/rterm/util/murmur_test.h
    include/rterm/util/named_args.h
    include/rterm/util/span.h
    include/rterm/util/string.h
    include/rterm/util/vector.h

    include/rterm/terminal/options.h
    include/rterm/terminal/terminal.h
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
    find_package(Freetype REQUIRED)
    find_package(Harfbuzz REQUIRED)
    list(APPEND RTERM_PRIVATE_LIBS Freetype::Freetype harfbuzz)
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

    list(APPEND RTERM_PUBLIC_LIBS CONAN_PKG::freetype CONAN_PKG::harfbuzz)
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

# utf8proc
set(UTF8PROC_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/utf8proc EXCLUDE_FROM_ALL)

# STB_Image
add_subdirectory(third_party/stb EXCLUDE_FROM_ALL)
list(APPEND RTERM_PRIVATE_LIBS stb_image)

# glad2
add_subdirectory(third_party/glad/cmake)

# expected & optional
list(APPEND RTERM_PUBLIC_HEADERS ${PROJECT_SOURCE_DIR}/third_party/optional/include/)
list(APPEND RTERM_PUBLIC_HEADERS ${PROJECT_SOURCE_DIR}/third_party/expected/include/)

# OpenGL
if (RTERM_CONFIG_GAPI_OPENGL)
    glad_add_library(glad_gl_33 STATIC EXCLUDE_FROM_ALL MX MXGLOBAL API gl:core=3.3 LANGUAGE c)
    list(APPEND RTERM_PRIVATE_LIBS glad_gl_33)
endif(RTERM_CONFIG_GAPI_OPENGL)

# Vulkan
if (RTERM_CONFIG_GAPI_VULKAN)
    glad_add_library(glad_vulkan_11 STATIC EXCLUDE_FROM_ALL MX MXGLOBAL API vulkan=1.1 LANGUAGE c)
    list(APPEND RTERM_PRIVATE_LIBS glad_vulkan_11)
endif(RTERM_CONFIG_GAPI_VULKAN)

# FontConfig
if (RTERM_CONFIG_SOURCE_FONTCONFIG)
    find_package(Fontconfig REQUIRED)
    list(APPEND RTERM_PRIVATE_LIBS Fontconfig::Fontconfig)
endif(RTERM_CONFIG_SOURCE_FONTCONFIG)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${RTERM_PRIVATE_HEADERS}
    PUBLIC ${RTERM_PUBLIC_HEADERS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${RTERM_PRIVATE_LIBS}
    PUBLIC ${RTERM_PUBLIC_LIBS}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    PUBLIC_HEADER "${HEADERS}"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_compile_definitions(
    ${PROJECT_NAME} PUBLIC
    RTERM_CONFIG_GAPI_OPENGL=$<BOOL:${RTERM_CONFIG_GAPI_OPENGL}>
    RTERM_CONFIG_GAPI_VULKAN=$<BOOL:${RTERM_CONFIG_GAPI_VULKAN}>
    RTERM_CONFIG_SOURCE_FONTCONFIG=$<BOOL:${RTERM_CONFIG_SOURCE_FONTCONFIG}>
    RTERM_CONFIG_SOURCE_DIRECTWRITE=$<BOOL:${RTERM_CONFIG_SOURCE_DIRECTWRITE}>
    RTERM_CONFIG_SOURCE_CORETEXT=$<BOOL:${RTERM_CONFIG_SOURCE_CORETEXT}>
    ${RTERM_OS}=1
)

if (RTERM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif(RTERM_BUILD_TESTS)
