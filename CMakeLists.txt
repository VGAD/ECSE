cmake_minimum_required(VERSION 3.6)

if (WIN32 OR CYGWIN)
  message(FATAL_ERROR "Don't use this on Windows, it's untested.")
endif()

add_subdirectory(ECSE)
add_subdirectory(ECSETest)

if (ECSE_BUILD_EXAMPLES)
  message(STATUS "BUILDING EXAMPLES")
  add_subdirectory("Bouncing Balls")
  add_subdirectory("Circle Collisions")
  add_subdirectory("Replay Demo")
  add_subdirectory("Simple Example")
endif(ECSE_BUILD_EXAMPLES)
