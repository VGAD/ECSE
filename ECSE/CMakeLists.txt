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
    State.cpp
    System.cpp
    TransformSystem.cpp
    World.cpp
    WorldState.cpp
)
message(STATUS "FILES: ${ecse_src}")

# Get boost and include it when we build
find_package(Boost REQUIRED)
set(includes ${Boost_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${includes})
message(STATUS "BOOST: ${Boost_INCLUDE_DIR}")
message(STATUS "includes: ${includes}")

add_library(ecse STATIC ${ecse_src})
TARGET_LINK_LIBRARIES(ecse ${Boost_LIBRARIES})
