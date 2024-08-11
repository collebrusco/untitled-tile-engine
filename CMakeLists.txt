cmake_minimum_required(VERSION 3.10)
project(untitled)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(lib/flgl)
add_subdirectory(lib/ecs)

include_directories(${PROJECT_SOURCE_DIR}/lib/flgl/inc ${PROJECT_SOURCE_DIR}/lib/sw ${PROJECT_SOURCE_DIR}/lib/ecs/inc ${PROJECT_SOURCE_DIR}/src)

file(GLOB PROJECT_SOURCES "src/*.cpp" "src/game/*.cpp" "src/util/*.cpp" "src/data/*.cpp" "src/rendering/*.cpp" "lib/sw/*.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-variable -Werror=unused-variable")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")

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

# Specify the source files for unit tests
file(GLOB UNITTEST_SOURCES "src/tests/*.cpp")

# Add a custom target for unit tests without the ALL keyword
add_custom_target(unittest
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${PROJECT_NAME}_unittest
    COMMAND cd ${CMAKE_CURRENT_LIST_DIR} && ./build/${PROJECT_NAME}_unittest
)

# Create the executable for unit tests only when the unittest target is built
add_executable(${PROJECT_NAME}_unittest ${PROJECT_SOURCES} ${UNITTEST_SOURCES})
target_compile_definitions(${PROJECT_NAME}_unittest PRIVATE UNITTEST)
target_link_libraries(${PROJECT_NAME}_unittest FLGL ecs)

set_target_properties(${PROJECT_NAME}_unittest PROPERTIES EXCLUDE_FROM_ALL TRUE EXCLUDE_FROM_DEFAULT_BUILD TRUE)
