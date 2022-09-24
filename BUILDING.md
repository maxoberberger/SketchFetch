# Building with CMake

## Dependencies

This Project uses vcpkg for dependency managment. Find the install instructions at:
[vcpkg Getting started](https://vcpkg.io/en/getting-started.html)


This project uses:
- [fmt](https://github.com/fmtlib/fmt) to build the search queries
- [nlohmann json](https://github.com/nlohmann/json) to parse the api results
- [restclient-cpp](https://github.com/mrtazz/restclient-cpp) to make requests to the Sketchfab API
- [libzippp](https://github.com/ctabin/libzippp) to unzip the downloadd archives from Sketchfab

For a list of dependencies, please refer to [vcpkg.json](vcpkg.json).

You need to install these dependencies with **vcpkg** and add the vcpkg toolchain file to cmake.
### Add this to your cmake args
```sh
 -DCMAKE_TOOLCHAIN_FILE={path_to}vcpkg/scripts/buildsystems/vcpkg.cmake
```
### Install the dependencies (on windows)
**vcpkg**
```sh
.\vcpkg.exe install fmt:x64-windows
.\vcpkg.exe install nlohmann-json:x64-windows
.\vcpkg.exe install restclient-cpp:x64-windows
.\vcpkg.exe install curl:x64-windows
```
**vcpkg** defaults to install libraries as x86, make sure to add x64-windows if your project targets x64


## Build

This project doesn't require any special command-line flags to build to keep
things simple.

Here are the steps for building in release mode with a single-configuration
generator, like the Unix Makefiles one:

```sh
cmake -S . -B build -D CMAKE_BUILD_TYPE=Release
cmake --build build
```

Here are the steps for building in release mode with a multi-configuration
generator, like the Visual Studio ones:

```sh
cmake -S . -B build
cmake --build build --config Release
```

### Building with MSVC

Note that MSVC by default is not standards compliant and you need to pass some
flags to make it behave properly. See the `flags-windows` preset in the
[CMakePresets.json](CMakePresets.json) file for the flags and with what
variable to provide them to CMake during configuration.

### Building on Apple Silicon

CMake supports building on Apple Silicon properly since 3.20.1. Make sure you
have the [latest version][1] installed.

## Install

This project doesn't require any special command-line flags to install to keep
things simple. As a prerequisite, the project has to be built with the above
commands already.

The below commands require at least CMake 3.15 to run, because that is the
version in which [Install a Project][2] was added.

Here is the command for installing the release mode artifacts with a
single-configuration generator, like the Unix Makefiles one:

```sh
cmake --install build
```

Here is the command for installing the release mode artifacts with a
multi-configuration generator, like the Visual Studio ones:

```sh
cmake --install build --config Release
```

### CMake package

This project exports a CMake package to be used with the [`find_package`][3]
command of CMake:

* Package name: `SketchFetch`
* Target name: `SketchFetch::SketchFetch`

Example usage:

```cmake
find_package(SketchFetch REQUIRED)
# Declare the imported target as a build requirement using PRIVATE, where
# project_target is a target created in the consuming project
target_link_libraries(
    project_target PRIVATE
    SketchFetch::SketchFetch
)
```

You can also use FetchContent to add SketchFetch to your Project:

```cmake
include(FetchContent)
FetchContent_Declare(
  SketchFetch
  GIT_REPOSITORY git@github.com:SpiRelli90/SketchFetch.git
  GIT_TAG Main
)
FetchContent_MakeAvailable(SketchFetch)
```

[1]: https://cmake.org/download/
[2]: https://cmake.org/cmake/help/latest/manual/cmake.1.html#install-a-project
[3]: https://cmake.org/cmake/help/latest/command/find_package.html
