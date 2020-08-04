cmake_minimum_required(VERSION 3.0)

project(kmath)

aux_source_directory(${PROJECT_SOURCE_DIR}/src KMATH_SOURCE)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_library(${PROJECT_NAME} SHARED ${KMATH_SOURCE})
add_library(${PROJECT_NAME}-static STATIC ${KMATH_SOURCE})
set_target_properties(${PROJECT_NAME}-static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})