cmake_minimum_required(VERSION 3.16)

# project name
project(CampusNavigation VERSION 0.1.0 LANGUAGES CXX)

# specify c++ standard to c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (APPLE)
    set(CMAKE_C_COMPILER "/usr/local/bin/gcc-10")
    set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-10")
endif(APPLE)

include_directories(src)
add_subdirectory(src)

enable_testing()
add_subdirectory(test)