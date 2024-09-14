cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 17)
project(boids)

enable_testing()

# doctest
set(DOCTEST_LIB doctest)
include_directories(lib ${DOCTEST_LIB})
set(doctest "${DOCTEST_LIB}/doctest.h")
add_library(doctest INTERFACE)

# sfml
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_subdirectory(src)
# add_subdirectory(test)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} Library sfml-graphics sfml-window sfml-system)
