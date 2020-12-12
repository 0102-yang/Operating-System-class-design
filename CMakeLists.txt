cmake_minimum_required(VERSION 3.16)

project(OS-class-design CXX)

include_directories(./include)

aux_source_directory(./src SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})