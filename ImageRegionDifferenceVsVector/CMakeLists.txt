cmake_minimum_required(VERSION 2.6)

PROJECT(ImageRegionDifferenceVsVector)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

ADD_EXECUTABLE(ImageRegionDifferenceVsVector ImageRegionDifferenceVsVector.cpp)
TARGET_LINK_LIBRARIES(ImageRegionDifferenceVsVector ${ITK_LIBRARIES})
