cmake_minimum_required(VERSION 3.12.0)

file(GLOB_RECURSE c_sources CONFIGURE_DEPENDS "*.c")
file(GLOB_RECURSE cpp_sources CONFIGURE_DEPENDS "*.cpp")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "*.h")

list(FILTER cpp_sources EXCLUDE REGEX "djgpp/.*$")
list(FILTER cpp_sources EXCLUDE REGEX "unix/.*$")
list(FILTER cpp_sources EXCLUDE REGEX "Win32/.*$")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	list(FILTER cpp_sources EXCLUDE REGEX ".*MethodTimer.cpp$")

	set(cpp_sources ${cpp_sources} "unix/MethodTimer.cpp")
	set(cpp_sources ${cpp_sources} "NullMethodTimer.cpp")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	list(FILTER cpp_sources EXCLUDE REGEX ".*TypeInformation.cpp$")
	
	set(cpp_sources ${cpp_sources} "Win32/TypeInformation.cpp")
endif()

source_group("Source files" FILES ${c_sources} ${cpp_sources})
source_group("Header files" FILES ${headers})

set(sources ${c_sources} ${cpp_sources} ${headers})

add_library(gppUnitSrc ${sources})

target_include_directories(gppUnitSrc PRIVATE ${PROJECT_SOURCE_DIR})

target_include_directories(gppUnitSrc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
