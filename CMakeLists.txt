cmake_minimum_required(VERSION 3.10)
set(ProjectName SocketsTest)
project(${ProjectName})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Додаємо підпроєкти
add_subdirectory(Client)
add_subdirectory(Network)
add_subdirectory(Server)
