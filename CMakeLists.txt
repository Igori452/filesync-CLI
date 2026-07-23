cmake_minimum_required(VERSION 3.10)

project(filesync-CLI)

set(CMAKE_CXX_STANDARD 20)

option(BUILD_TESTS "Build tests" ON)

if (BUILD_TESTS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
    
    add_subdirectory(lib/googletest)
endif()

add_subdirectory(src/core)
add_subdirectory(src/CommandLineParser)
