cmake_minimum_required(VERSION 3.16)

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

if (UNIX)
set(CMAKE_CXX_STANDARD 17)
target_compile_options(ircb PUBLIC -std=c++17 -g)
elseif(WIN32)

#add_executable(ircb client.cpp)
#add_executable(parse_test irc_protocol_tests.cpp)

#add_library(irc OBJECT irc.cpp)
#target_link_libraries(parse_test gtest_main irc)
#target_link_libraries(ircb irc)

target_compile_options(ircb PRIVATE "/std:c++20")
#target_compile_options(irc PRIVATE "/std:c++20")
target_compile_options(parse_test PRIVATE "/std:c++20")

endif()
