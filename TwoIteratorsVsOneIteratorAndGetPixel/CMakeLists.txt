cmake_minimum_required(VERSION 2.6)

PROJECT(TwoIteratorsVsOneIteratorAndGetPixel)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

ADD_EXECUTABLE(TwoIteratorsVsOneIteratorAndGetPixel TwoIteratorsVsOneIteratorAndGetPixel.cpp)
TARGET_LINK_LIBRARIES(TwoIteratorsVsOneIteratorAndGetPixel ${ITK_LIBRARIES})
