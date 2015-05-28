ECSE
====
A (very) basic Entity-Component System game engine.

Building and running
====================
ECSE was built using Visual Studio Community 2013. You can build it by
opening `ECSE.sln`.
Note that the following libraries are required:

- SFML 2.2 (statically compiled and installed in `C:\Program Files (x86)\SFML`)
- Boost 1.57.0 (installed in `C:\boost_1_57_0`)
- For unit testing, Google Test (installed in `C:\gtest-1.7.0` with `msvc\gtest-md.sln` built)

If you have these libraries installed in a different location, modify the
settings for your game project. Include directories are set in
`Configuration Properties -> C/C++ -> General`, while library directories
are set in `Configuration Properties -> Linker -> General`.

If you want to run the example make sure the ECSE project setting for
`Configuration Properties -> Debugging -> Working Directory` is set to
`$(SolutionDir)Run` and one of the Example configurations.

Contribution Conventions
========================
* Unless you're a repo admin, pull request everything, even if it's a simple change.
* Before submitting a PR, run the unit tests and confirm that they're passing.

Coding Conventions
==================
* No "naked news." Any time memory is allocated it should be wrapped in a smart pointer (even if it's just going to be passed to another function).
* The owner should always maintain a smart pointer. Non-owners should use a regular pointer.
* References should be used when passing objects that must exists (i.e. in cases where the pointer will never be null).
* Null pointers should be set to `nullptr`, **not** 0 or `NULL`.