cmake_minimum_required(VERSION 3.12.0)

add_library(gppUnit INTERFACE)
target_link_libraries(gppUnit INTERFACE gppUnitApprovalsTest gppUnitMatchers gppUnitSrc gppUnitTableFormatting)
target_include_directories(gppUnit INTERFACE ${PROJECT_SOURCE_DIR})
