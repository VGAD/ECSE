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

# Get boost and include it when we build
find_package(Boost REQUIRED system filesystem)
include_directories(${Boost_INCLUDE_DIR})
message(STATUS "BOOST: ${Boost_INCLUDE_DIR}")
message(STATUS "BOOST LIB: ${Boost_LIBRARIES}")

# Get SFML and include it when we build
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2 REQUIRED COMPONENTS audio graphics window system )
message(STATUS "SFML FOUND ${SFML_FOUND}")
message(STATUS "SFML LIBRARIES ${SFML_LIBRARIES}")
message(STATUS "SFML DEPENDENCIES ${SFML_DEPENDENCIES}")

include_directories(${ECSE_INCLUDE_DIR})
add_executable(ecse_test ${ecse_test_src})
TARGET_LINK_LIBRARIES(
  ecse_test
    ecse
    ${GTEST_LIBRARIES}
    ${Boost_LIBRARIES}
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES}
    pthread
)
