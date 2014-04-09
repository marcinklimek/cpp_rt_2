cmake_minimum_required(VERSION 2.6)

project(socketlib)

if(CMAKE_SOURCE_DIR STREQUAL socketlib_SOURCE_DIR)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x -Wall -Wextra")
endif()

set(sources
	net/base_socket.cpp
	net/socket_utils.cpp
	net/internet_socket.cpp
	net/tcp_socket.cpp
	net/udp_socket.cpp
	net/unix_socket.cpp
	net/unix_datagram_socket.cpp)

include_directories( ${CMAKE_SOURCE_DIR}/net )

source_group(net_src FILES ${sources})
add_library(net STATIC ${sources})


set(threads
        thread/launch_thread.cpp
        thread/threadpool.cpp)

include_directories( ${CMAKE_SOURCE_DIR}/thread )

source_group(thread_src FILES ${threads})
add_library(thread STATIC ${threads})

file(GLOB libs_SRC ${CMAKE_SOURCE_DIR}"thread/*.h")
add_custom_target(libs SOURCES ${libs_SRC})

set(examples
    tcp_client
    tcp_server
    tcp_server_threaded
    unix_client
    unix_server
    )

foreach(EX ${examples})

    add_executable(${EX} ${EX}.cpp )
    target_link_libraries(${EX} net thread pthread)
    message("* ${EX}")

endforeach()
