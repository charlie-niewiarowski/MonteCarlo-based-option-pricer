cmake_minimum_required(VERSION 3.16)

project(MonteCarloSim)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable SIMD
add_compile_options(-O3 -march=native -mavx2 -mfma)

include(ExternalProject)
find_package(Git REQUIRED)

# External build of SLEEF
ExternalProject_Add(libsleef
        GIT_REPOSITORY https://github.com/shibatch/sleef.git
        GIT_TAG master
        CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/contrib
        -DENABLE_INLINE_HEADERS:BOOL=ON
        -DENABLE_AVX2:BOOL=ON
        BUILD_BYPRODUCTS
        ${CMAKE_BINARY_DIR}/contrib/lib/libsleef.a
)

# Tell your compiler where to find SLEEF
include_directories(${CMAKE_BINARY_DIR}/contrib/include)
link_directories(${CMAKE_BINARY_DIR}/contrib/lib)

# Main target
add_executable(MonteCarloSim
        main.cpp
        lib/Timer.cpp
        lib/Timer.h
        montecarlo.cpp
        montecarlo.h
)

# Ensure SLEEF builds before your executable
add_dependencies(MonteCarloSim libsleef)

# Link against SLEEF
target_link_libraries(MonteCarloSim PRIVATE sleef)
