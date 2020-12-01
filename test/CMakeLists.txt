cmake_minimum_required(VERSION 3.12.0)

file(GLOB_RECURSE c_sources CONFIGURE_DEPENDS "*.c")
file(GLOB_RECURSE cpp_sources CONFIGURE_DEPENDS "*.cpp")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "*.h")

list(FILTER cpp_sources EXCLUDE REGEX "Win32/.*$")

if (MSVC)
	list(FILTER cpp_sources EXCLUDE REGEX ".*TestMethodTimer.cpp$")

	set(cpp_sources ${cpp_sources} "Win32/TestMethodTimer.cpp")
	set(cpp_sources ${cpp_sources} "Win32/TestQuotesToParentheses.cpp")
endif()

source_group("Source files" FILES ${c_sources} ${cpp_sources})
source_group("Header files" FILES ${headers})

set(sources ${c_sources} ${cpp_sources} ${headers})

add_executable(gppUnitTest ${sources})

target_include_directories(gppUnitTest PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(gppUnitTest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(gppUnitTest PUBLIC gppUnit)

file(GLOB_RECURSE approved_files CONFIGURE_DEPENDS "*.approved.txt")
foreach(approved_file ${approved_files})
	configure_file(${approved_file} ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
endforeach()

file(GLOB_RECURSE check_file_contents_files CONFIGURE_DEPENDS "*CheckFileContents.txt")
foreach(check_file_contents_file ${check_file_contents_files})
	configure_file(${check_file_contents_file} ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
endforeach()
