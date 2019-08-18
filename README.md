MemoizedMember
===============

MemoizedMember is a C++ class that caches the result of a computation.
Use it when you have a class with an attribute or property
that is (relatively) expensive to compute and not always queried
or queried a lot.

# Usage

If your class `A` has an attribute `b` with a value you want to memoize:

1. Write a `private`, `const` computation method, `type compute_b() const`.

2. Add a `MemoizedMember` member using the computation method.

3. Write a `public` getter for the `MemoizedMember`, `type get_b() const`.

4. When an object is mutated enough to invalidate the memoized value, call its `reset` method.

Here is an example memoizing an `int` type attribute.

``` C++
class A
{
  public:
    int get_b() const { return b; }
    void mutate() { b.reset(); }
  private:
    int compute_b() const;
    MemoizedMember<int, A, &A::compute_b> b{*this};
};
```

# Installation

If you want to use `MemoizedMember` in another CMake project
use the following block in your `CMakeLists.txt`

``` CMake
# Download and unpack memoized-member at configure time
configure_file(CMakeLists.memoized-member.in memoized-member-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" -DCMAKE_INSTALL_PREFIX=../memoized-member-install .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/memoized-member-download)
if(result)
  message(FATAL_ERROR "CMake step for MemoizedMember failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/memoized-member-download)
if(result)
  message(FATAL_ERROR "Build step for MemoizedMember failed: ${result}")
endif()

include(${CMAKE_CURRENT_BINARY_DIR}/memoized-member-install/cmake/MM.cmake)
```

And put the following contents in a file called `CMakeLists.memoized-member.in`.
Update the `GIT_TAG` property as desired to choose the version to include.
`GIT_TAG` can be set to either a branch or tag name.

```
cmake_minimum_required(VERSION 2.8.2)

project(memoized-member-download NONE)

include(ExternalProject)
ExternalProject_Add(memoized-member
  GIT_REPOSITORY    https://github.com/Pafnoutios/memoized-member.git
  GIT_TAG           origin/master
  SOURCE_DIR        "${CMAKE_CURRENT_BINARY_DIR}/memoized-member-src"
  BINARY_DIR        "${CMAKE_CURRENT_BINARY_DIR}/memoized-member-build"
  INSTALL_DIR       "${CMAKE_CIRRENT_BINARY_DIR}/memoized-member-install"
  CMAKE_ARGS        "-DCMAKE_INSTALL_PREFIX=../memoized-member-install"
)
```

Your project will now automatically download the desired version of this library,
build and install it into your build director,
and use it as an imported CMake target.
The imported target is `MemoizedMember`.
Just `target_link_libraries` against it and its include directory will be imported into your targets include search path.

```cmake
target_link_libraries(my_target MemoizedMember)
```

```c++
#include <MemoizedMember.hpp>
```
