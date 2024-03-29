cmake_minimum_required(VERSION 3.16)

project(ircb)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS --coverage)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/main.zip
)

FetchContent_MakeAvailable(googletest)

add_executable( ircb client.cpp irc.cpp )
target_link_libraries( ircb )
include_directories(include)

add_executable( parse_test irc_protocol_tests.cpp irc.cpp )
target_link_libraries( parse_test gtest_main gcov )

include(GoogleTest)

if (UNIX)
    set(CMAKE_CXX_STANDARD 17)
    target_compile_options(ircb PUBLIC -std=c++17 -g  )
# compile options for coverage checks via gcov
    target_compile_options(parse_test PUBLIC -std=c++17 -g --coverage -O0 )

elseif(WIN32)
    target_compile_options(ircb PRIVATE "/std:c++20")
    target_compile_options(parse_test PRIVATE "/std:c++20")
endif()
