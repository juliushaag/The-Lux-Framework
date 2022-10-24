cmake_minimum_required(VERSION 3.23)

project(Sandbox)

set(SRC_FILES "src/main.cpp" ../LuxEngine/src/Core/Window.cpp ../LuxEngine/src/Core/Window.h)

set(HEADER_FILES "")


message(${PROJECT_SOURCE_DIR}/LuxEngine/src/)

add_executable(Sandbox ${SRC_FILES} ${HEADER_FILES})

target_include_directories(Sandbox PRIVATE ../LuxEngine/src/)


target_link_libraries(Sandbox PUBLIC Lux)