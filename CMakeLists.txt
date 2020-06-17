
# GEnum is a tool to generate enum to string functions automatically from the source.
# This makes sure names of the enums are always up to date.

cmake_minimum_required(VERSION 3.10)

project(GEnum CXX)

### Require out-of-source builds
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()



option(BUILD_TESTS
	"Build test programs and test targets"
	OFF
)



# No reason not to compile using multiple cores.
if(MSVC)
	add_compile_options(
		"/MP"
	)
endif()

# Enable folder separators for targets.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)



set(PROJECT_FILES
	"Source/Main.cpp"
	"Source/GEnum.cpp"
	"Source/GEnum.h"
)

add_executable(GEnum
	${PROJECT_FILES}
)

set_target_properties(GEnum
	PROPERTIES
		CXX_STANDARD				17
		ARCHIVE_OUTPUT_DIRECTORY	"${CMAKE_BINARY_DIR}/Lib"
		LIBRARY_OUTPUT_DIRECTORY	"${CMAKE_BINARY_DIR}/Lib"
		RUNTIME_OUTPUT_DIRECTORY	"${CMAKE_BINARY_DIR}/Bin"
)



if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	if(BUILD_TESTS)
		include(CTest)
		add_subdirectory("Tests")
	endif()
endif()
