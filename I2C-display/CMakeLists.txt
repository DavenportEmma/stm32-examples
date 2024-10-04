cmake_minimum_required(VERSION 3.27.3)

include(${SDK_BASE}/cmake/common.cmake)

project(i2c-display C)

target_sources(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/i2c.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ssd1306.c
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)