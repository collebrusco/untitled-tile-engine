cmake_minimum_required(VERSION 3.10)
project(untitled)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(lib/flgl)

include_directories(${PROJECT_SOURCE_DIR}/lib/flgl/inc ${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE PROJECT_SOURCES "src/*.cpp" "lib/sw/*.cpp")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} FLGL)

add_custom_target(c
    COMMAND ${CMAKE_COMMAND} -E remove -f untitled
    COMMAND ${CMAKE_COMMAND} -E remove -Rf CMakeFiles/untitled.dir/src/*
    COMMENT "Cleaned application code"
)

add_custom_target(cleancm
    COMMAND ${CMAKE_COMMAND} -E remove -f CMakeCache.txt cmake_install.cmake 
    COMMENT "Cleaned cmake cache"
)
