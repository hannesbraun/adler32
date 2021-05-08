cmake_minimum_required(VERSION 2.8)
project(adler32 C)

set(CMAKE_C_STANDARD 90)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(adler32
        adler32.c)
