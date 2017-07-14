project("ecse_replay_demo")

set(
  replay_demo_src
    main.cpp
    Bindings.cpp
    ControllableBall.cpp
    ReplayDemo.cpp
)

include_directories(${ECSE_INCLUDE_DIR})
add_executable(ecse_replay_demo ${replay_demo_src})
target_link_libraries(
  ecse_replay_demo
    ecse
)
