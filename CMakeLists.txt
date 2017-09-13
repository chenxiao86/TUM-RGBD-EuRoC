
project(TUM22EuRoC)
cmake_minimum_required(VERSION 2.8)


SET(CMAKE_CXX_COMPILER "g++")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
endif ()

SET( CMAKE_BUILD_TYPE Debug  )
#SET( CMAKE_BUILD_TYPE release  )


ADD_EXECUTABLE(main main.cpp)
TARGET_LINK_LIBRARIES( main
    )
