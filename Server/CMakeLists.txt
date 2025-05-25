cmake_minimum_required(VERSION 3.10)
set(ProjectName Server)
project(${ProjectName})

add_executable(${ProjectName} src/main.cpp)


add_custom_command(TARGET ${ProjectName} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_CURRENT_SOURCE_DIR}/res $<TARGET_FILE_DIR:${ProjectName}>/res)
target_link_libraries(Server ws2_32 Network)
