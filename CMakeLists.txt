cmake_minimum_required(VERSION 3.23)
project(Lux)

set(CMAKE_CXX_STANDARD 20)
set(LUX_PROJECT_SOURCE_DIR "@PROJECT_SOURCE_DIR@")

find_package(Vulkan REQUIRED)

add_subdirectory(LuxEngine)

add_subdirectory(Sandbox)

