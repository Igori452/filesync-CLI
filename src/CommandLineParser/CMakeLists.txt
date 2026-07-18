cmake_minimum_required(VERSION 3.10)

project(CommandLineParser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

file(GLOB CPPS "src/*.cpp")

add_library(${PROJECT_NAME} ${CPPS})

# INCLUDE CORE MODULE
target_link_libraries(${PROJECT_NAME} core)

################# TESTS #################
if (BUILD_TESTS)
    file(GLOB TESTS "../../tests/CommandLineParser/*.cpp" 
                    "../../tests/main.cpp")

    add_executable(${PROJECT_NAME}_tests ${TESTS})

    # INCLUDE GOOGLETEST LIBRARY
    target_link_libraries(${PROJECT_NAME}_tests
        ${PROJECT_NAME}
        gtest
    )
endif()