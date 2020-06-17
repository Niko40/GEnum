
# GEnum is a tool to generate enum to string functions automatically from the source.
# This makes sure names of the enums are always up to date.

cmake_minimum_required(VERSION 3.10)



set(TEST_FILES
	"Test.cpp"
	"TestEnums/TestEnums.h"
	"../Source/GEnum.cpp"
	"../Source/GEnum.h"
)

add_executable(Test
	${TEST_FILES}
)

set_target_properties(Test
	PROPERTIES
		CXX_STANDARD				17
		ARCHIVE_OUTPUT_DIRECTORY	"${CMAKE_BINARY_DIR}/Lib"
		LIBRARY_OUTPUT_DIRECTORY	"${CMAKE_BINARY_DIR}/Lib"
		RUNTIME_OUTPUT_DIRECTORY	"${CMAKE_BINARY_DIR}/Bin"
)

target_compile_definitions(Test
	PRIVATE
		TEST_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/TestEnums"
)

add_test(NAME TESTBED
	COMMAND Test
)

set_tests_properties(TESTBED PROPERTIES TIMEOUT 10)
