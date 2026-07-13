cmake_minimum_required(VERSION 3.10)

project(core)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB CPPS "src/*.cpp")

add_library(${PROJECT_NAME} ${CPPS})

################# TESTS #################
option (BUILD_TESTS OFF)

if (BUILD_TESTS)
    add_executable(${PROJECT_NAME}_tests ../../tests/core/*.cpp)

    target_link_libraries(${PROJECT_NAME}_tests
        ${PROJECT_NAME}
        gtest
    )
endif()