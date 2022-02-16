cmake_minimum_required(VERSION 3.16)

project(ircb)

add_executable( ircb client.cpp )

target_compile_options(ircb PUBLIC -std=c++17 -g)
    
