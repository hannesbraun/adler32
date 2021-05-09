cmake_minimum_required(VERSION 2.8.12)
project(adler32 C)

set(CMAKE_C_STANDARD 90)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(adler32 adler32.c)

if (UNIX)
    install(TARGETS adler32 DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
    install(FILES adler32.1 DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1")
endif()
