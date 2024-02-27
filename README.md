# examples_cpp
Examples of C++17/20 code / also Boost v1.82

# install packages
##conan install conanfile.py --build=missing
conan install . --output-folder=build --build=missing

cd build
# Linux OS
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
# Windows OS
cmake .. -G "Visual Studio 16 2019" -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake

# build 
cmake --build . --config Release
