# DungeonZ

This repository features a 2D game done with SFML and CMake, in C++, with the sole purpose of training my coding skills
in this language (game oriented-wise).

The concept or premise for this game was to create a simple 2D crawler game.

## How to Use

1. Clone this repository.
2. Open the scripts directory in the DungeonZ root folder.
3. Run the 'generate.bat' batch script to generate the CMake project and solution.
4. Run the 'build.bat' batch script to build the project.
5. Access the <TO DO look up the build directory> build folder.
6. Run the <TO DO look up the executable> game executable.
7. Enjoy!

## Upgrading SFML

SFML is found via CMake's [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html) module.
FetchContent automatically downloads SFML from GitHub and builds it alongside your own code.
Beyond the convenience of not having to install SFML yourself, this ensures ABI compatibility and simplifies things like
specifying static versus shared libraries.

Modifying what version of SFML you want is as easy as changing the `GIT_TAG` argument.
Currently it uses SFML 3 via the `3.0.0` tag.

## But I want to...

Modify CMake options by adding them as configuration parameters (with a `-D` flag) or by modifying the contents of
CMakeCache.txt and rebuilding.

### Not use GitHub

You can use this project without a GitHub account
by [downloading the contents](https://github.com/SFML/cmake-sfml-project/archive/refs/heads/master.zip) of the
repository as a ZIP archive and unpacking it locally.
This approach also avoids using Git entirely if you would prefer to not do that.

### Change Compilers

See the variety of [`CMAKE_<LANG>_COMPILER`](https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER.html)
options.
In particular you'll want to modify `CMAKE_CXX_COMPILER` to point to the C++ compiler you wish to use.

### Change Compiler Optimizations

CMake abstracts away specific optimizer flags through the [
`CMAKE_BUILD_TYPE`](https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html) option.
By default this project recommends `Release` builds which enable optimizations.
Other build types include `Debug` builds which enable debug symbols but disable optimizations.
If you're using a multi-configuration generator (as is often the case on Windows), you can modify the [
`CMAKE_CONFIGURATION_TYPES`](https://cmake.org/cmake/help/latest/variable/CMAKE_CONFIGURATION_TYPES.html#variable:CMAKE_CONFIGURATION_TYPES)
option.

### Change Generators

While CMake will attempt to pick a suitable default generator, some systems offer a number of generators to choose from.
Ubuntu, for example, offers Makefiles and Ninja as two potential options.
For a list of generators, click [here](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html).
To modify the generator you're using you must reconfigure your project providing a `-G` flag with a value corresponding
to the generator you want.
You can't simply modify an entry in the CMakeCache.txt file unlike the above options.
Then you may rebuild your project with this new generator.

## More Reading

Here are some useful resources if you want to learn more about CMake:

- [Official CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/)
- [How to Use CMake Without the Agonizing Pain - Part 1](https://alexreinking.com/blog/how-to-use-cmake-without-the-agonizing-pain-part-1.html)
- [How to Use CMake Without the Agonizing Pain - Part 2](https://alexreinking.com/blog/how-to-use-cmake-without-the-agonizing-pain-part-2.html)
- [Better CMake YouTube series by Jefferon Amstutz](https://www.youtube.com/playlist?list=PL8i3OhJb4FNV10aIZ8oF0AA46HgA2ed8g)

## License

The source code is dual licensed under Public Domain and MIT -- choose whichever you prefer.
