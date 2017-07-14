ECSE
====
A (very) basic Entity-Component System game engine.

Building and running on Windows
===============================
ECSE was built using Visual Studio Community 2013. You can build it by opening `ECSE.sln`.
Note that the following libraries are required, and will need to be compiled from source:

- SFML 2.4.2 (statically compiled and installed in `C:\Program Files (x86)\SFML`)
- Boost 1.64.0 (installed in `C:\boost_1_64_0`)
- For the ECSETest unit testing project, Google Test (installed in `C:\Program Files (x86)\googletest`;

If you have these libraries installed in a different location, modify the settings for your game project. Include
directories are set in `Configuration Properties -> C/C++ -> General`, while library directories are set in
`Configuration Properties -> Linker -> General`.

If you try to run any of the executable projects and get a missing DLL error, navigate to
`Configuration Properties -> Debugging` for the project, then set `Environment` to `PATH=%PATH%;$(SolutionDir)\DLL`.
This will add the necessary DLL(s) to your program's PATH while it's being run.

If you're going to be running the engine in debug mode, especially if you're making use of the collision detection
system, it is strongly recommended to use the `Debug (No Iterator Debugging)` target instead of `Debug`. This requires
that you recompile the dependencies with `_ITERATOR_DEBUG_LEVEL=0` defined. While a bit painful to set up, this
significantly improves debug performance by removing Visual Studio's largely redundant iterator debugging checks.

Building and running on Linux
=============================
ECSE is confirmed to work on at least Ubuntu 16.04 LTS.

First install prequisite libraries:
* libboost-all-dev

Then clone and build SFML (static):
```bash
git clone git@github.com:SFML/SFML.git <sfml source path>
mkdir <sfml build path>
cd <sfml build path>

# Install dependencies
sudo apt-get install \
  libfreetype6-dev libjpeg9-dev libxrandr-dev libx11-dev libxcb1-dev \
  libx11-xcb-dev libxcb-randr0-dev libxcb-image0-dev freeglut3-dev libflac-dev \
  libogg-dev libvorbis-dev libvorbisenc2 libvorbisfile3 libopenal-dev \
  libpthread-workqueue-dev libudev-dev

# Other build types are `RelWithDebInfo`, `Debug`, or `MinSizeRel`
# Would recommend `RelWithDebInfo` for debugging unless you start having issues
# in gdb where things are "optimized out" in which case you should choose
# `Debug`
# Can ignore Examples and Docs.
# Turn off build shared libs to build static
cmake <sfml source path> \
    -DCMAKE_BUILD_TYPE=Release \
    -DSFML_BUILD_EXAMPLES=False \
    -DSFML_BUILD_DOC=False \
    -DBUILD_SHARED_LIBS=False

make -j <threads>
sudo make install
```

Install and build gtest:
```bash
sudo apt-get install libgtest-dev
cd /usr/src/gtest/

# Sudo required
sudo cmake .
sudo make -j <threads>
sudo cp libgtest.a /usr/local/lib
sudo cp libgtest_main.a /usr/local/lib
```

Now build ECSE:
```bash
git clone git@github.com:VGAD/ECSE.git <ECSE source path>
mkdir <ECSE build path>
cd <ECSE build path>
cmake <ECSE source path> \
  -DCMAKE_MODULE_PATH="/usr/local/share/SFML/cmake/Modules/" \
  -DCMAKE_CXX_FLAGS="--std=c++14" \
  -DCMAKE_BUILD_TYPE="Release"
make -j <threads>
sudo make install
```

Some useful variables:
* `-DCMAKE_CXX_COMPILER=<your favorite compiler>` -> Remember that this
  should be something like clang++ or g++ not their C counterparts
* `-DCMAKE_CXX_FLAGS="<a string of your desired compiler flags>"`
* `-DCMAKE_BUILD_TYPE=[Release\MinSizeRel\RelWithDebInfo\Debug]`
* `-DECSE_BUILD_EXAMPLES=True` -> Build the ECSE example projects
* More if required: https://cmake.org/Wiki/CMake_Useful_Variables

Note that `c++14` is minimally required to compile ECSE and so `--std=c++14`
is required to be in your list of compiler flags.

To use ECSE in your project with cmake:
```cmake
# Find ECSE package
find_package(ECSE REQUIRED)
message(STATUS "ECSE FOUND: ${ECSE_FOUND}")

# Include the headers
include_directories(${ECSE_INCLUDE_DIRS})

# Link the library to your target
target_link_libraries(${target_name} ${ECSE_LIBRARIES})
```

Contribution Conventions
========================
* Unless you're a repo admin, pull request everything, even if it's a simple change.
* Before submitting a PR, run the unit tests and confirm that they're passing.
* If you're adding new functionality, make sure to add corresponding unit tests as well.

Coding Conventions
==================
* No "naked news." Any time memory is allocated it should be wrapped in a smart pointer (even if it's just going to be passed to another function).
* The owner of allocated memory should always maintain a smart pointer to that memory so it is automatically deallocated. Non-owners should use a regular pointer.
* References should be used when passing objects that must exist (i.e. in cases where the pointer will never be null).
* Null pointers should be set to `nullptr`, **not** 0 or `NULL`.
