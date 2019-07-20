#general project rules
cmake_minimum_required(VERSION 3.1)
project(Visual)

#cpp
set_target_properties(PROPERTIES CXX_STANDARD 17)

#sources
add_executable(Visual
        source/main.cpp
        source/logger/logger.cpp
        source/logger/logger.hpp
        source/config/config.hpp
        source/state/state.cpp
        source/state/state.hpp
        source/manager/manager.hpp
        source/manager/texture_manager.cpp
        source/manager/texture_manager.hpp
        source/perlin_noise/perlin_noise.hpp
        source/perlin_noise/perlin_noise.cpp
        source/math/math.hpp
        source/effect/effect_manager.cpp
        source/effect/effect_manager.hpp
        source/flag_manager/flag_manager.cpp
        source/flag_manager/flag_manager.hpp
        source/utils/utils.hpp
        source/resource_guard/resource_guard.hpp
        source/resource_collection/resource_collection.hpp)

#sfml
set(SFML_DIR "external/sfml/lib/cmake/SFML")
find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
target_link_libraries(Visual sfml-graphics sfml-audio)

#boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.65.1 REQUIRED COMPONENTS filesystem)
target_link_libraries(Visual Boost::filesystem)

#gtest
add_subdirectory(gtest/lib/googletest-master)
include_directories(gtest/lib/googletest-master/googletest/include)
include_directories(gtest/lib/googletest-master/googlemock/include)
target_link_libraries(Visual gtest gtest_main)

#opencv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(Visual ${OpenCV_LIBS})

#threadpool
include_directories(external/threadpool)
