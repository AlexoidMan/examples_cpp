cmake_minimum_required(VERSION 3.15)

if(POLICY CMP0074)
    # find_package() uses <PackageName>_ROOT variables.
    # This policy was introduced in CMake version 3.12.
    cmake_policy(SET CMP0074 NEW)
endif()

project(tasks)

set(list_desctructor "list_desctructor")

#common

set(list_sources
    "list_destructor.cpp"
)

set(list_headers
) 

add_executable(list_desctructor ${list_sources} ${list_headers} )
target_include_directories(list_desctructor PUBLIC include)

