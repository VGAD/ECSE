Magnaut
=======

An explosive top-down shoot-em-up in which the hulls of defeated enemy ships are
your only weapons and armor.

Building and running
====================
Magnaut was built using Visual Studio Community 2013. You can build it by
opening `Magnaut.sln`.
Note that the following libraries are required:

- SFML 2.2 (statically compiled and installed in `C:\Program Files (x86)\SFML`)
- Boost 1.57.0 (installed in `C:\boost_1_57_0`)

If you have these libraries installed in a different location, modify the
settings for the Magnaut project. Include directories are set in
`Configuration Properties -> C/C++ -> General`, while library directories
are set in `Configuration Properties -> Linker -> General`.

Also note that the application will fail to launch by default as the following
files must be included in the run directory (e.g. if you're running the Release
build, the Release directory):

- All the DLLs in the `DLLs` directory (`libsndfile-1.dll` and `openal32.dll`)
- A copy of the `resources` directory

You may consider using symbolic links to avoid a lot of tedious copying and
pasting if you're planning to modify resources. Run these commands as an
administrator from the run directory:
```
mklink libsndfile-1.dll ..\DLLs\libsndfile-1.dll
mklink openal32.dll ..\DLLs\openal32.dll
mklink /D resources ..\resources
```

Coding Conventions
==================
* No "naked news." Any time memory is allocated it should be wrapped in a smart pointer (even if it's just going to be passed to another function).
* The owner should always maintain a smart pointer. Non-owners should use a regular pointer.
* References should be used when passing objects that must exists (i.e. in cases where the pointer will never be null).
* Null pointers should be set to `nullptr`, **not** 0 or `NULL`.