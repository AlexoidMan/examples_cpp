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
set(CMAKE_CXX_EXTENSIONS ON)

#Boost requirements
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS thread system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
# Found libraries saved to Boost_LIBRARIES variable
link_directories(${Boost_LIBRARY_DIR})

#common
find_package (Threads)


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

add_executable(coroYieldBack "src/coroutine/coroyieldback.cpp" "src/coroutine/coroyieldback.hpp" "src/coroutine/corogenback.hpp" "src/coroutine/backawaiter.hpp" ) 

# move semantics
add_executable(emplace_copy "src/move/emplace_copy.cpp") 

# concurrent 
add_executable(quickSort_parallel "src/concurrent/quickSort_parallelized.cpp")
add_executable(promise_test "src/concurrent/promise.cpp")
add_executable(future_exp_test "src/concurrent/future.cpp")
target_link_libraries(future_exp_test  Boost::thread)
#target_link_libraries(future_exp_test  ${Boost_LIBRARY_DIR})

# metaprogramming
add_executable(declval_ex1 "src/metaprogramming/declval.cpp")
add_executable(container_polymorphic "src/metaprogramming/container_polymorphic.cpp")

# patterns
add_executable(abstract_factory "src/patterns/abstract_factory.cpp")

# tasks
add_subdirectory("src/tasks")

# jthread
add_subdirectory("src/jthread")

# templates semantics
add_subdirectory("src/templates")

# iterators
add_subdirectory("src/iterator") 

#set_target_properties(hello PROPERTIES PUBLIC_HEADER "include/hello.h")
install(TARGETS coro_main1)
