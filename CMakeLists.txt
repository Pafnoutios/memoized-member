# Memoized Member
# © John Szwast

cmake_minimum_required(VERSION 2.8)

project(MemoizedMember)

add_subdirectory(googletest)


set(headers MemoizedMember.hpp)

set(sources)

source_group("Header Files" FILES ${headers})
source_group("Source Files" FILES ${sources})


