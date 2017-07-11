project("ecse_circle_collisions")

set(
  circle_collisions_src
    main.cpp
    CircleCollisionTest.cpp
    TestUtil.cpp
)

include_directories(${ECSE_INCLUDE_DIR})
add_executable(ecse_circle_collisions ${circle_collisions_src})
target_link_libraries(
  ecse_circle_collisions
    ecse
)
