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
* libsfml-dev
* libboost-dev

Then run cmake in your desired build directory. Some useful flags:
* `-DCMAKE_CXX_COMPILER=<your favorite compiler>`
* `-DCMAKE_CXX_FLAGS="<a string of your desired compiler flags>"`

Note that `c++14` is minimally required to compile ECSE and so `--std=c++14`
is required to be in your list of compiler flags.

An example build on ubuntu might look like this
```
git clone git@github.com:VGAD/ECSE.git <path to ECSE>
sudo apt-get install libsfml-dev libboost-dev
mkdir <path to build directory>
cd <path to build directory>
cmake -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_CXX_FLAGS="--std=c++14" <path to ECSE>
make -j <thread count>```

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
