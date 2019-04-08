cmake_minimum_required(VERSION 3.1)
project(Visual)
set(SFML_DIR "external/lib/cmake/SFML")
find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
add_executable(Visual
        source/main.cpp)

target_link_libraries(Visual sfml-graphics sfml-audio)
