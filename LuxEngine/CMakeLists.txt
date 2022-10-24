cmake_minimum_required(VERSION 3.23)

project(LuxEngine)

set(SRC_FILES "src/Core/Application.cpp")

set(HEADER_FILES "src/Core/Application.h" src/Utils/Types.h)

add_library(Lux STATIC ${SRC_FILES} ${HEADER_FILES})

target_include_directories(Lux PRIVATE src/)