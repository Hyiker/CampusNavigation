cmake_minimum_required(VERSION 3.13)

# project name
project(CampusNavigation VERSION "0.1.0")

# specify c++ standard to c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# find sources
aux_source_directory(src SOURCE_DIR)

# set target
add_executable(${PROJECT_NAME} ${SOURCE_DIR})
target_link_libraries(${PROJECT_NAME})

enable_testing()
add_subdirectory(test)