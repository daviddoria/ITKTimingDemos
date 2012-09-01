cmake_minimum_required(VERSION 2.6)

PROJECT(NeighborhoodIterator)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

ADD_EXECUTABLE(NeighborhoodIterator NeighborhoodIterator.cpp)
TARGET_LINK_LIBRARIES(NeighborhoodIterator ${ITK_LIBRARIES})
