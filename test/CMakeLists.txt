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

add_executable(test ${sources})

target_include_directories(test PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(test PUBLIC ApprovalsTest Matchers TableFormatting src)
