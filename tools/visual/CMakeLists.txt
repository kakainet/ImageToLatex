#general project rules
cmake_minimum_required(VERSION 3.1)
project(visual)

#cpp
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17 -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++17 -O0")

#sources
add_executable(visual
        source/main.cpp
        source/logger/logger.cpp
        source/state/state.cpp
        source/perlin_noise/perlin_noise.cpp
        source/effect/effect_manager.cpp
        source/flag_manager/flag_manager.cpp
        source/transform/transform.cpp
        source/image_manager/image_manager.cpp
        source/attributes/reportable.cpp
        source/tests/math_test.cc)

#boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.65.1 REQUIRED COMPONENTS filesystem)
target_link_libraries(visual Boost::filesystem)

#gtest
add_subdirectory(external/googletest/googletest)
include_directories(external/googletest/googletest/include)
target_link_libraries(visual gtest)

#opencv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(visual ${OpenCV_LIBS})

#threadpool
include_directories(external/threadpool)

#doxygen
find_package(Doxygen)
if (DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            )
endif (DOXYGEN_FOUND)

