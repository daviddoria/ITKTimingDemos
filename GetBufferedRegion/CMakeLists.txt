cmake_minimum_required(VERSION 2.6)

PROJECT(GetBufferedRegion)

FIND_PACKAGE(ITK REQUIRED ITKCommon)
INCLUDE(${ITK_USE_FILE})

ADD_EXECUTABLE(GetBufferedRegion GetBufferedRegion.cpp)
TARGET_LINK_LIBRARIES(GetBufferedRegion ${ITK_LIBRARIES})
