cmake_minimum_required(VERSION 3.16)

# find sources
file(GLOB SOURCES
    main.cc
    algo/navigation.h
    algo/navigation.cc
    logger/logger.h
    logger/logger.cc
)

# set target
add_executable(${PROJECT_NAME}_run ${SOURCES})
add_library(${PROJECT_NAME}_lib STATIC ${SOURCES})
