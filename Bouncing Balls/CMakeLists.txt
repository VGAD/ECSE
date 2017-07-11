project("ecse_bouncing_balls")

set(
  bouncing_balls_src
    main.cpp
      BallSystem.cpp
      Bindings.cpp
      BouncingBalls.cpp
      LineRenderSystem.cpp
)

include_directories(${ECSE_INCLUDE_DIR})
add_executable(ecse_bouncing_balls ${bouncing_balls_src})
target_link_libraries(
  ecse_bouncing_balls
    ecse
)
