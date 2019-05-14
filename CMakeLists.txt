cmake_minimum_required(VERSION 3.6)

if (WIN32 OR CYGWIN)
  message(FATAL_ERROR "Don't use this on Windows, it's untested.")
endif()

# Set up our destination directories
set(ecse_dest "${CMAKE_INSTALL_PREFIX}/lib/ecse")
set(include_dest "${CMAKE_INSTALL_PREFIX}/include/ecse")

# Set C++ standards.
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(ECSE)
add_subdirectory(ECSETest)

if (ECSE_BUILD_EXAMPLES)
  message(STATUS "BUILDING EXAMPLES")
  add_subdirectory("Bouncing Balls")
  add_subdirectory("Circle Collisions")
  add_subdirectory("Replay Demo")
  add_subdirectory("Simple Example")
endif(ECSE_BUILD_EXAMPLES)

# We need to have a relative include dir from where the config file is to the
# include files, cmake can change this to be relative to an including project
# later
file(
  RELATIVE_PATH # Find a relative path
  relative_inc_path # Destination variable
  ${ecse_dest} # From
  ${include_dest} # To
)

# Set the template variable in the Config.cmake file
set(exported_inc_dir "\${this_dir}/${relative_inc_path}")
configure_file(ECSEConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ECSEConfig.cmake" @ONLY)

# Install the files
install(FILES "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ECSEConfig.cmake"
        DESTINATION ${ecse_dest})
install(EXPORT ecse
  FILE ECSETargets.cmake
  DESTINATION ${ecse_dest}
)
