cmake_minimum_required(VERSION 3.4)
project(gender_age_model)

SET(CMAKE_C_COMPILER "/usr/bin/gcc")

add_definitions(-std=c++11)
add_definitions(-fPIC)
add_definitions(-Wall)
add_definitions(-O2)

set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(LINK_DIR "${PROJECT_SOURCE_DIR}/lib")

link_directories(${LINK_DIR})

include_directories(${INCLUDE_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

aux_source_directory(src SRCS)

if ($ENV{PREDICTOR__EXECUTABLE})
    message("ENV PREDICTOR__EXECUTABLE is Defined, make an executable app")
    add_executable(${PROJECT_NAME} main.cpp ${SRCS})
else()
    message("ENV PREDICTOR__EXECUTABLE is NOT Defined, make a shared lib")
    add_library(${PROJECT_NAME} SHARED ${SRCS})
endif()

target_link_libraries(${PROJECT_NAME}
    mxnet
    pthread
    opencv_core 
    opencv_highgui 
    opencv_imgcodecs
    opencv_imgproc
)

