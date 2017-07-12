project(ecse)

set(
  ecse_src
    AnimationSet.cpp
    CollisionDebugSystem.cpp
    CollisionMath.cpp
    CollisionSystem.cpp
    Common.cpp
    Engine.cpp
    Entity.cpp
    EntityManager.cpp
    InputManager.cpp
    Logging.cpp
    PrefabManager.cpp
    RenderSystem.cpp
    SpecializationSystem.cpp
    Spritemap.cpp
    State.cpp
    System.cpp
    TagSystem.cpp
    TransformSystem.cpp
    World.cpp
    WorldState.cpp
)

# Set up our include dir for the PUBLIC.
# Things including us will want "ECSE/<file>.h"
# TODO provide FindECSE.cmake file for external projects to include
get_filename_component(parent_dir ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
set(ECSE_INCLUDE_DIR "${parent_dir}"
      CACHE PATH "ECSE's include directory")
message(STATUS "SET ECSE INCLUDE DIR: ${ECSE_INCLUDE_DIR}")

# Internally none of the files include using "ECSE/<file>.h", so just include
# the current working directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Get boost and include it when we build
find_package(Boost REQUIRED system filesystem)
include_directories(${Boost_INCLUDE_DIR})
message(STATUS "BOOST: ${Boost_INCLUDE_DIR}")

# Get SFML and include it when we build
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2 REQUIRED COMPONENTS audio graphics network window system )
message(STATUS "SFML FOUND ${SFML_FOUND}")
message(STATUS "SFML LIBRARIES ${SFML_LIBRARIES}")
message(STATUS "SFML DEPENDENCIES ${SFML_DEPENDENCIES}")

add_library(ecse STATIC ${ecse_src})
target_link_libraries(
  ecse
    ${Boost_LIBRARIES}
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES}
    pthread
)
