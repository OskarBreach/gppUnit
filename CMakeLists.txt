cmake_minimum_required (VERSION 3.12.0)

project ("GppUnit")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(ApprovalsTest)
add_subdirectory(Matchers)
add_subdirectory(src)
add_subdirectory(TableFormatting)
add_subdirectory(gppUnit)

add_subdirectory(test)
