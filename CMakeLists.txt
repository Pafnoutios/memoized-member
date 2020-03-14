#	\author    John Szwast
#	\year      2014-2019
#	\copyright MIT

cmake_minimum_required(VERSION 3.2)

project(MemoizedMember)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

enable_testing()

set(doc README.md)
set(headers include/MemoizedMember.hpp)
set(sources)
set(unit_tests test/mm_test.cpp)


add_library(MemoizedMember INTERFACE)
target_include_directories(MemoizedMember INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

install(TARGETS MemoizedMember EXPORT MM)
install(EXPORT MM DESTINATION cmake)
install(FILES ${headers} DESTINATION include)
install(FILES ${doc} DESTINATION doc)

add_executable(mm_tests ${unit_tests})

target_link_libraries(mm_tests MemoizedMember ${CONAN_LIBS})

add_test(NAME MemoizedMember-Unit-Tests COMMAND mm_tests)


