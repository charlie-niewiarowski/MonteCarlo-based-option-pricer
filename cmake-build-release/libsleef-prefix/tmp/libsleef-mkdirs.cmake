# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/cniew/CLionProjects/MonteCarloSim/cmake-build-release/libsleef-prefix/src/libsleef"
  "/home/cniew/CLionProjects/MonteCarloSim/cmake-build-release/libsleef-prefix/src/libsleef-build"
  "/home/cniew/CLionProjects/MonteCarloSim/cmake-build-release/libsleef-prefix"
  "/home/cniew/CLionProjects/MonteCarloSim/cmake-build-release/libsleef-prefix/tmp"
  "/home/cniew/CLionProjects/MonteCarloSim/cmake-build-release/libsleef-prefix/src/libsleef-stamp"
  "/home/cniew/CLionProjects/MonteCarloSim/cmake-build-release/libsleef-prefix/src"
  "/home/cniew/CLionProjects/MonteCarloSim/cmake-build-release/libsleef-prefix/src/libsleef-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/cniew/CLionProjects/MonteCarloSim/cmake-build-release/libsleef-prefix/src/libsleef-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/cniew/CLionProjects/MonteCarloSim/cmake-build-release/libsleef-prefix/src/libsleef-stamp${cfgdir}") # cfgdir has leading slash
endif()
