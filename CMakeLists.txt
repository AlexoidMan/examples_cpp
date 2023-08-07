cmake_minimum_required(VERSION 3.15)

if(POLICY CMP0074)
    # find_package() uses <PackageName>_ROOT variables.
    # This policy was introduced in CMake version 3.12.
    cmake_policy(SET CMP0074 NEW)
endif()

project(C++_examples CXX)

# C++ standard requirements.
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(cor_sources
    "src/coroutine/coro_main1.cpp"
)

set(cor_headers
    "src/coroutine/coro.hpp"
    "src/coroutine/corotask.hpp"
    "src/coroutine/corotaskpromise.hpp"
)


add_executable(coro_main1 ${cor_sources} ${cor_headers} )
target_include_directories(coro_main1 PUBLIC include)

add_executable(cororange "src/coroutine/cororange.cpp" "src/coroutine/cororange.hpp" "src/coroutine/generator.hpp") 

# move semantics
add_executable(emplace_copy "src/move/emplace_copy.cpp") 

# templates semantics
add_executable(template_variadic "src/templates/template_variadic.cpp") 

#set_target_properties(hello PROPERTIES PUBLIC_HEADER "include/hello.h")
install(TARGETS coro_main1)
