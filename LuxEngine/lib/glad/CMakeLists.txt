cmake_minimum_required(VERSION 3.15)
project(glad)


set(GLAD_SRC_FILES src/glad.c)
set(GLAD_HEADER_FILES include/glad/glad.h include/KHR/khrplatform.h)


add_library(glad ${GLAD_SRC_FILES} ${GLAD_HEADER_FILES})

target_include_directories(glad PRIVATE include/)
