cmake_minimum_required(VERSION 3.10)

project(mem_dumper)

add_library(mem_dumper src/memory_dumper.cpp)

target_include_directories(mem_dumper PUBLIC include/
                                    PRIVATE include/mem_dumper
                                    PRIVATE src/)
set_property(TARGET mem_dumper PROPERTY CXX_STANDARD 17)
set_property(TARGET mem_dumper PROPERTY CXX_STANDARD_REQUIRED ON)

add_subdirectory(test)