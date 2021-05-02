cmake_minimum_required(VERSION 3.10)

project(ggr)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()
 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(source)
include_directories(libraries)

add_subdirectory(source)

add_executable(ggr source/ggr.cpp)
target_link_libraries(ggr Core)

add_subdirectory(examples)

