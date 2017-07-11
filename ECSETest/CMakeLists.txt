project(ecse_test)

set(
  ecse_test_src
    main.cpp
    TestCollisionMath.cpp
    TestCollisionSystem.cpp
    TestCommon.cpp
    TestEngine.cpp
    TestEntityManager.cpp
    TestFixtures.h
    TestInputManager.cpp
    TestPrefabManager.cpp
    TestSpecialization.cpp
    TestState.cpp
    TestSystem.cpp
    TestTransformSystem.cpp
    TestUtils.h
    TestVectorMath.cpp
    TestWorld.cpp
)

message(STATUS "FILES: ${ecse_test_src}")
message(STATUS "ECSE: ${ECSE_INCLUDE_DIR}")

# Get GTest and include it when we build
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
message(STATUS "GTEST: ${GTEST_INCLUDE_DIRS}")

include_directories(${ECSE_INCLUDE_DIR})
add_executable(ecse_test ${ecse_test_src})
target_link_libraries(
  ecse_test
    ecse
    ${GTEST_LIBRARIES}
)
