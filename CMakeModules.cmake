function(add_project_module MODULE_NAME)
    project(${MODULE_NAME})

    file(GLOB CPPS "src/*.cpp")

    add_library(${MODULE_NAME} ${CPPS})
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

    target_link_libraries(${MODULE_NAME} CoreUtility ${ARGN})

    if (BUILD_TESTS)
        file(GLOB TESTS "../../tests/${MODULE_NAME}/*.cpp" "../../tests/main.cpp")
        
        if(TESTS)
            add_executable(${MODULE_NAME}_tests ${TESTS})
            target_link_libraries(${MODULE_NAME}_tests
                ${MODULE_NAME}
                gtest
            )
        endif()
    endif()
endfunction()