cmake_minimum_required(VERSION 3.16)

# find sources
file(GLOB_RECURSE SOURCES *.cc)
file(GLOB_RECURSE HEADERS *.h)
file(GLOB_RECURSE HPPS *.hpp)


# set target
add_executable(${PROJECT_NAME}_run ${SOURCES} ${HEADERS} ${HPPS})
add_library(${PROJECT_NAME}_lib STATIC ${SOURCES})

include_directories(${PROJECT_SOURCE_DIR}/ext/http) 
include_directories(${PROJECT_SOURCE_DIR}/ext/json)

target_link_libraries(${PROJECT_NAME}_run ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT_NAME}_lib ${Boost_LIBRARIES})

if(MSVC)
  target_compile_options(${PROJECT_NAME}_run PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME}_run PRIVATE -Wall -Wextra -pedantic)
endif()
