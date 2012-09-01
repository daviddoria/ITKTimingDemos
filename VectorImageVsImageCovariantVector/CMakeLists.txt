cmake_minimum_required(VERSION 2.6)

PROJECT(VectorImageVsImageCovariantVector)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

ADD_EXECUTABLE(VectorImageVsImageCovariantVector VectorImageVsImageCovariantVector.cpp)
TARGET_LINK_LIBRARIES(VectorImageVsImageCovariantVector ${ITK_LIBRARIES})
