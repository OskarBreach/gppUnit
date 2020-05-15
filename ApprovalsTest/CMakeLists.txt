cmake_minimum_required(VERSION 3.12.0)

file(GLOB_RECURSE c_sources CONFIGURE_DEPENDS "*.c")
file(GLOB_RECURSE cpp_sources CONFIGURE_DEPENDS "*.cpp")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "*.h")

source_group("Source files" FILES ${c_sources} ${cpp_sources})
source_group("Header files" FILES ${headers})

set(sources ${c_sources} ${cpp_sources} ${headers})

add_library(ApprovalsTest ${sources})
