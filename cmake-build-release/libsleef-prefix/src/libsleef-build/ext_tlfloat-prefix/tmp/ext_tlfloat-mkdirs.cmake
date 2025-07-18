# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/cniew/CLionProjects/MonteCarloSim/cmake-build-release/libsleef-prefix/src/libsleef/submodules/tlfloat"
  "/home/cniew/CLionProjects/MonteCarloSim/cmake-build-release/libsleef-prefix/src/libsleef-build/ext_tlfloat-prefix/src/ext_tlfloat-build"
  "/home/cniew/CLionProjects/MonteCarloSim/cmake-build-release/libsleef-prefix/src/libsleef-build/ext_tlfloat-prefix"
  "/home/cniew/CLionProjects/MonteCarloSim/cmake-build-release/libsleef-prefix/src/libsleef-build/ext_tlfloat-prefix/tmp"
  "/home/cniew/CLionProjects/MonteCarloSim/cmake-build-release/libsleef-prefix/src/libsleef-build/ext_tlfloat-prefix/src/ext_tlfloat-stamp"
  "/home/cniew/CLionProjects/MonteCarloSim/cmake-build-release/libsleef-prefix/src/libsleef-build/ext_tlfloat-prefix/src"
  "/home/cniew/CLionProjects/MonteCarloSim/cmake-build-release/libsleef-prefix/src/libsleef-build/ext_tlfloat-prefix/src/ext_tlfloat-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/cniew/CLionProjects/MonteCarloSim/cmake-build-release/libsleef-prefix/src/libsleef-build/ext_tlfloat-prefix/src/ext_tlfloat-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/cniew/CLionProjects/MonteCarloSim/cmake-build-release/libsleef-prefix/src/libsleef-build/ext_tlfloat-prefix/src/ext_tlfloat-stamp${cfgdir}") # cfgdir has leading slash
endif()
