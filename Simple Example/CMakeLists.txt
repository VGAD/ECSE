project("ecse_simple_example")

set(
  simple_example_src
    main.cpp
)

include_directories(${ECSE_INCLUDE_DIR})
add_executable(ecse_simple_example ${simple_example_src})
target_link_libraries(
  ecse_simple_example
    ecse
)

# Copy assets
file(COPY text.png DESTINATION ${PROJECT_BINARY_DIR})
