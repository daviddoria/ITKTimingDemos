cmake_minimum_required(VERSION 2.6)

PROJECT(ConditionalVsFull)

FIND_PACKAGE(ITK REQUIRED ITKCommon)
INCLUDE(${ITK_USE_FILE})

ADD_EXECUTABLE(ConditionalVsFull ConditionalVsFull.cpp)
TARGET_LINK_LIBRARIES(ConditionalVsFull ${ITK_LIBRARIES})
