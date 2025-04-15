cmake_minimum_required(VERSION 3.12)

project(
    STL-Adaptors-Flow
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

include_directories(lib)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
