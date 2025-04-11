cmake_minimum_required(VERSION 3.27.3)

include(${SDK_BASE}/cmake/common.cmake)

project(midi C)

target_sources(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)