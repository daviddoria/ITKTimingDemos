cmake_minimum_required(VERSION 2.6)

PROJECT(GetPixelVsIterator)

FIND_PACKAGE(ITK REQUIRED ITKCommon)
INCLUDE(${ITK_USE_FILE})

ADD_EXECUTABLE(GetPixelVsIterator GetPixelVsIterator.cpp)
TARGET_LINK_LIBRARIES(GetPixelVsIterator ${ITK_LIBRARIES})
