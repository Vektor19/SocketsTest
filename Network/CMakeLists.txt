cmake_minimum_required(VERSION 3.10)
project(Network)

add_library(Network STATIC
	src/Network.cpp
	src/Network.h
	src/SocketHandle.h
	src/EIpVersion.h
	src/EResult.h
	src/ESocketOption.h
	src/Socket.h
	src/Socket.cpp
	src/IpEndpoint.h
	src/IpEndpoint.cpp
)
target_include_directories(Network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
