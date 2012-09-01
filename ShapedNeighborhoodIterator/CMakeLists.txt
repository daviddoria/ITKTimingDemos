cmake_minimum_required(VERSION 2.6)

PROJECT(ShapedNeighborhoodIterator)

FIND_PACKAGE(ITK REQUIRED ITKCommon)
INCLUDE(${ITK_USE_FILE})

ADD_EXECUTABLE(ShapedNeighborhoodIterator ShapedNeighborhoodIterator.cpp)
TARGET_LINK_LIBRARIES(ShapedNeighborhoodIterator ${ITK_LIBRARIES})
