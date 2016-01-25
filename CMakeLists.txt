#	\author    John Szwast
#	\year      2014-2016
#	\copyright MIT

cmake_minimum_required(VERSION 2.8)

project(MemoizedMember)

add_subdirectory(googletest)

enable_testing()

include_directories(googletest/include)

set(doc README.md)
set(headers MemoizedMember.hpp)
set(sources)
set(unit_tests mm_test.cpp)


source_group("Documentation" FILES ${doc})
source_group("Header Files" FILES ${headers})
source_group("Source Files" FILES ${sources})
source_group("Unit Tests" FILES ${unit_tests})

add_executable(mm_tests ${headers} ${unit_tests} ${doc})
target_compile_options(mm_tests PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:--std=c++11>)
target_link_libraries(mm_tests gtest_main)

add_test(NAME MemoizedMember-Unit-Tests COMMAND mm_tests)


