cmake_minimum_required(VERSION 3.10)

project(core)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB CPPS "src/*.cpp")

add_library(${PROJECT_NAME} ${CPPS})

################# TESTS #################
if (BUILD_TESTS)
    file(GLOB TESTS "../../tests/core/*.cpp" 
                    "../../tests/main.cpp")

    add_executable(${PROJECT_NAME}_tests ${TESTS})

    target_link_libraries(${PROJECT_NAME}_tests
        ${PROJECT_NAME}
        gtest
    )
endif()