cmake_minimum_required(VERSION 3.15)

if(POLICY CMP0074)
    # find_package() uses <PackageName>_ROOT variables.
    # This policy was introduced in CMake version 3.12.
    cmake_policy(SET CMP0074 NEW)
endif()

project(iterators)


add_executable(iterator1 iterator_1.cpp )


