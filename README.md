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

If you have these libraries installed in a different location, modify the
settings for the Magnaut project. Include directories are set in
`Configuration Properties -> C/C++ -> General`, while library directories
are set in `Configuration Properties -> Linker -> General`.

After opening the solution, make sure the ECSE project setting for
`Configuration Properties -> Debugging -> Working Directory` is set to ..\Run if
you want to see the example run.

Coding Conventions
==================
* No "naked news." Any time memory is allocated it should be wrapped in a smart pointer (even if it's just going to be passed to another function).
* The owner should always maintain a smart pointer. Non-owners should use a regular pointer.
* References should be used when passing objects that must exists (i.e. in cases where the pointer will never be null).
* Null pointers should be set to `nullptr`, **not** 0 or `NULL`.