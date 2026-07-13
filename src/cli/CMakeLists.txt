cmake_minimum_required(VERSION 3.10)

project(cli)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../core
                 ${CMAKE_CURRENT_BINARY_DIR}/core)

file(GLOB CPPS "src/*.cpp")

add_library(${PROJECT_NAME} ${CPPS})

# INCLUDE CORE MODULE
target_link_libraries(${PROJECT_NAME} core)

################# TESTS #################
option (BUILD_TESTS OFF)

if (BUILD_TESTS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../lib/googletest 
                     ${CMAKE_CURRENT_BINARY_DIR}/googletest)

    add_executable(${PROJECT_NAME}_tests ../../tests/cli/*.cpp)

    # INCLUDE GOOGLETEST LIBRARY
    target_link_libraries(${PROJECT_NAME}_tests
        ${PROJECT_NAME}
        gtest
    )
endif()