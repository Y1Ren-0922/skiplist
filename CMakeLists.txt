cmake_minimum_required(VERSION 3.22)
project(skiplist)

set(CMAKE_CXX_STANDARD 17)

add_executable(skiplist main.cpp skiplist.hpp shell_command.h shell_command.cpp  utils/my_string.h type.h)
#shell_command.h shell_command.cpp  utils/my_string.h type.h
#target_compile_definitions(skiplist PUBLIC TEST)

