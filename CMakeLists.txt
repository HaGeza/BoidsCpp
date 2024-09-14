cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 17)
project(boids)

enable_testing()

# sfml
find_package(SFML 2.6.1 COMPONENTS graphics window system REQUIRED)

# doctest
set(DOCTEST_LIB doctest)
set(doctest "${DOCTEST_LIB}/doctest.h")
add_library(doctest INTERFACE)

# include directories
include_directories(lib ${DOCTEST_LIB})

add_subdirectory(src)
# add_subdirectory(test)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} Library)
