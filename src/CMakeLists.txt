cmake_minimum_required(VERSION 3.16)

# find sources
file(GLOB_RECURSE SOURCES *.cc)
file(GLOB_RECURSE HEADERS *.h)

# set target
add_executable(${PROJECT_NAME}_run ${SOURCES} ${HEADERS})
add_library(${PROJECT_NAME}_lib STATIC ${SOURCES})

target_link_libraries(${PROJECT_NAME}_run ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_lib ${Boost_LIBRARIES})