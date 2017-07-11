cmake_minimum_required(VERSION 3.6)

add_subdirectory(ECSE)
add_subdirectory(ECSETest)

if (ECSE_BUILD_EXAMPLES)
  message(STATUS "BUILDING EXAMPLES")
  add_subdirectory("Bouncing Balls")
  add_subdirectory("Circle Collisions")
  add_subdirectory("Replay Demo")
  add_subdirectory("Simple Example")
endif(ECSE_BUILD_EXAMPLES)
