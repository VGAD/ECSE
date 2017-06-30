set(
  ecse_src
    #Engine.cpp
    Entity.cpp
    # PrefabManager.cpp
    State.cpp
    World.cpp
)
message(STATUS "FILES: ${ecse_src}")

# Get bost and include it when we build
find_package(Boost REQUIRED)
set(includes ${Boost_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${includes})
message(STATUS "BOOST: ${Boost_INCLUDE_DIR}")
message(STATUS "includes: ${includes}")

add_library(ecse STATIC ${ecse_src})
TARGET_LINK_LIBRARIES(ecse ${Boost_LIBRARIES})
