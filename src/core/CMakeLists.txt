project(CoreUtility)

file(GLOB CPPS "src/*.cpp")

add_library(${PROJECT_NAME} ${CPPS})

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(BUILD_TESTS OFF)

if (BUILD_TESTS)
    file(GLOB TESTS "../../tests/${MODULE_NAME}/*.cpp" "../../tests/main.cpp")
    
    if(TESTS)
        add_executable(${PROJECT_NAME}_tests ${TESTS})
        target_link_libraries(${PROJECT_NAME}_tests
            ${PROJECT_NAME}
            gtest
        )
    endif()
endif()