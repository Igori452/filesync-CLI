cmake_minimum_required(VERSION 3.10)

project(Logger)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

file(GLOB CPPS "src/*.cpp")

add_library(${PROJECT_NAME} ${CPPS})

# INCLUDE CORE MODULE AND DEPENDENCIES
target_link_libraries(${PROJECT_NAME} CoreUtility OutputInterface)

################# TESTS #################
if (BUILD_TESTS)
    file(GLOB TESTS "../../tests/${PROJECT_NAME}/*.cpp" 
                    "../../tests/main.cpp")

    add_executable(${PROJECT_NAME}_tests ${TESTS})

    # INCLUDE GOOGLETEST LIBRARY
    target_link_libraries(${PROJECT_NAME}_tests
        ${PROJECT_NAME}
        gtest
    )
endif()