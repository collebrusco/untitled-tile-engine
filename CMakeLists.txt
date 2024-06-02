cmake_minimum_required(VERSION 3.10)
project(untitled)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(lib/flgl)
add_subdirectory(lib/ecs)

include_directories(${PROJECT_SOURCE_DIR}/lib/flgl/inc ${PROJECT_SOURCE_DIR}/lib/ecs/inc ${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE PROJECT_SOURCES "src/*.cpp" "lib/sw/*.cpp")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} FLGL)
target_link_libraries(${PROJECT_NAME} ecs)

add_custom_target(c
    COMMENT "Removing untitled"
    COMMAND ${CMAKE_COMMAND} -E remove -f untitled
    COMMENT "Removing src files"
    COMMAND rm -rf CMakeFiles/untitled.dir/src/*.o
    COMMAND rm -rf CMakeFiles/untitled.dir/src/**/*.o
    COMMENT "Cleaned application code"
)


add_custom_target(cleancm
    COMMAND ${CMAKE_COMMAND} -E remove -f CMakeCache.txt cmake_install.cmake 
    COMMENT "Cleaned cmake cache"
)
