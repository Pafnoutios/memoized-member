#	\author    John Szwast
#	\year      2014-2016
#	\copyright MIT

cmake_minimum_required(VERSION 2.8)

project(MemoizedMember)

# Download and unpack googletest at configure time
configure_file(CMakeLists.googletest.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)


enable_testing()

set(doc README.md)
set(headers MemoizedMember.hpp)
set(sources)
set(unit_tests mm_test.cpp)


source_group("Documentation" FILES ${doc})
source_group("Header Files" FILES ${headers})
source_group("Source Files" FILES ${sources})
source_group("Unit Tests" FILES ${unit_tests})

add_executable(mm_tests ${headers} ${unit_tests} ${doc})
set_property(TARGET mm_tests PROPERTY CXX_STANDARD 11)
set_property(TARGET mm_tests PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(mm_tests gtest_main)

add_test(NAME MemoizedMember-Unit-Tests COMMAND mm_tests)


