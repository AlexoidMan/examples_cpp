cmake_minimum_required(VERSION 3.15)

if(POLICY CMP0074)
    # find_package() uses <PackageName>_ROOT variables.
    # This policy was introduced in CMake version 3.12.
    cmake_policy(SET CMP0074 NEW)
endif()

project(C++_examples CXX)

if(UNIX AND NOT APPLE)
    # use RPATH instead of RUNPATH so that
    # transitive dependencies can be located
    add_link_options("LINKER:--disable-new-dtags")
endif()

if(BOOST_NAMESPACE)
    add_definitions("-DBOOST_NAMESPACE=${BOOST_NAMESPACE}")
endif()

# C++ standard requirements.
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

#Boost requirements
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.82 COMPONENTS thread system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
# Found libraries saved to Boost_LIBRARIES variable
link_directories(${Boost_LIBRARY_DIR})

#common
find_package (Threads)
find_package(benchmark REQUIRED)

include_directories(${benchmark_INCLUDE_DIRS})
link_directories(${benchmark_LIB_DIRS_RELEASE}) # links without errors

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
add_executable(stackTrace_test "src/concurrent/stackTrace.cpp")
target_link_libraries(stackTrace_test PRIVATE Boost::stacktrace_backtrace)

#add_executable(future_exp_test "src/concurrent/future.cpp")
#target_link_libraries(future_exp_test PRIVATE Boost::headers Boost::chrono Boost::thread)
#target_link_libraries(future_exp_test  ${Boost_LIBRARY_DIR})

# metaprogramming
add_executable(declval_ex1 "src/metaprogramming/declval.cpp")
add_executable(container_polymorphic "src/metaprogramming/container_polymorphic.cpp")

# patterns

#message(STATUS ${benchmark_INCLUDE_DIRS})
#message(STATUS ${benchmark_LIBRARIES})
add_executable(abstract_factory "src/patterns/abstract_factory.cpp")
add_executable(type_erasure "src/patterns/type_erasure.cpp")
target_link_libraries(type_erasure  PUBLIC benchmark)

add_executable(crtp_generic_interfaces "src/patterns/CRTP_generic_interfaces.cpp")
add_executable(crtp_policy_design "src/patterns/CRTP_policy_designed.cpp")
add_executable(crtp_template_policy "src/patterns/CRTP_policy_template_designed.cpp")

add_executable(fluent_builder "src/patterns/fluentBuilder.cpp")
add_executable(implicit_builder "src/patterns/implicitBuilder.cpp")
add_executable(fluent_builder_hierarchy "src/patterns/fluentBuilderHierarchy.cpp")

add_executable(localBufferOpt "src/patterns/LBO.cpp")
add_executable(localBufferOpt_vector "src/patterns/LBO_vector.cpp")
target_link_libraries(localBufferOpt  PUBLIC benchmark)
target_link_libraries(localBufferOpt_vector  PUBLIC benchmark)

add_executable(scope_guard "src/patterns/scope_guard.cpp")
#add_executable(scope_guard_te "src/patterns/scope_guard_te.cpp")

add_executable(factory "src/patterns/FactoryPattern.cpp")
add_executable(factory_serialization "src/patterns/Factory_serialization.cpp")

add_executable(policy1 "src/patterns/Policy_pattern.cpp")

add_executable(decorator1 "src/patterns/decorator.cpp")


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
