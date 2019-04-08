cmake_minimum_required(VERSION 3.1)
project(Visual)
set(SFML_DIR "external/sfml/lib/cmake/SFML")
set_target_properties(PROPERTIES CXX_STANDARD 17)
find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
add_executable(Visual
        source/main.cpp
        source/logger/logger.cpp
        source/logger/logger.hpp
        source/config/config.hpp
        source/cache/cache.hpp)

target_link_libraries(Visual sfml-graphics sfml-audio)
