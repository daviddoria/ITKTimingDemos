cmake_minimum_required(VERSION 2.6)

PROJECT(IteratorWithIndex)

FIND_PACKAGE(ITK REQUIRED ITKCommon)
INCLUDE(${ITK_USE_FILE})

ADD_EXECUTABLE(IteratorWithIndex IteratorWithIndex.cpp)
TARGET_LINK_LIBRARIES(IteratorWithIndex ${ITK_LIBRARIES})
