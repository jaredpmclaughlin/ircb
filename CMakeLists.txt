cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

project(ircb)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/main.zip
)

FetchContent_MakeAvailable(googletest)

add_executable( ircb client.cpp irc.cpp )



add_executable( parse_test irc_protocol_tests.cpp irc.cpp )
target_link_libraries( parse_test gtest_main )

include(GoogleTest)

target_compile_options(ircb PUBLIC -std=c++17 -g)
    
