project(launch_thread)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries( ${PROJECT_NAME}  pthread)
add_definitions("-std=c++11")

file(GLOB libs_SRC "*.h" "*.hpp")
add_custom_target(libs SOURCES ${libs_SRC})