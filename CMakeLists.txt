cmake_minimum_required(VERSION 3.10.0)
project(TP_Coursework VERSION 0.1.0 LANGUAGES C CXX)

enable_testing()

add_library(TP_Coursework
    directed_graph.cpp
    directed_graph.h
)


# GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Автоматический поиск тестов
file(GLOB TEST_SOURCES "tests/*.cpp")

# Создание отдельных исполняемых файлов для каждого теста
foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    
    add_executable(${test_name} ${test_source})
    
    target_link_libraries(${test_name}
        PRIVATE 
        TP_Coursework
        GTest::gtest_main
    )
    
    add_test(
        NAME ${test_name}
        COMMAND ${test_name}
    )
endforeach()
