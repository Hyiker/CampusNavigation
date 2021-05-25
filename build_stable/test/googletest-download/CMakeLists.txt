cmake_minimum_required(VERSION 3.13)

include(ExternalProject)
ExternalProject_Add(googletest
  URL               /mnt/c/Users/16078/Desktop/CampusNavigation/ext/googletest
#  GIT_TAG           release-1.10.0
  SOURCE_DIR        "/mnt/c/Users/16078/Desktop/CampusNavigation/build/test/googletest-src"
  BINARY_DIR        "/mnt/c/Users/16078/Desktop/CampusNavigation/build/test/googletest-build"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)
