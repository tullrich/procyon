# CMakeLists.txt for Procyon test suite
add_subdirectory(lib/gmock-1.7.0)

enable_testing()

include_directories(
	${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR}
	${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR}
)

add_executable(runUnitTests
	tests/test_main.cpp
	tests/reflection_test.cpp
	tests/ioc_test.cpp
)

target_link_libraries(runUnitTests
	Procyon
	gtest
	gmock
)

add_test(NAME that-test-I-made COMMAND runUnitTests)
