cmake_minimum_required(VERSION 3.15)

if(POLICY CMP0074)
    # find_package() uses <PackageName>_ROOT variables.
    # This policy was introduced in CMake version 3.12.
    cmake_policy(SET CMP0074 NEW)
endif()

project(templates_examples)


add_executable(template_variadic template_variadic.cpp )
add_executable(template_partial template_partial.cpp )
add_executable(template_template_parameter template_template_parameter.cpp )
add_executable(template_variadic_lambdas template_variadic_lambdas.cpp )

#add_executable(ex2 ex2.cpp )
#target_include_directories(template_variadic PUBLIC include)

