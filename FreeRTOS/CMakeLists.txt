cmake_minimum_required(VERSION 3.27.3)

include(${SDK_BASE}/cmake/common.cmake)

project(freertos C)

target_sources(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${SDK_BASE}/modules/FreeRTOS/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c
    ${SDK_BASE}/modules/FreeRTOS/FreeRTOS/Source/portable/MemMang/heap_4.c
    ${SDK_BASE}/modules/FreeRTOS/FreeRTOS/Source/tasks.c
    # ${SDK_BASE}/modules/FreeRTOS/FreeRTOS/Source/croutine.c
    # ${SDK_BASE}/modules/FreeRTOS/FreeRTOS/Source/event_groups.c
    ${SDK_BASE}/modules/FreeRTOS/FreeRTOS/Source/list.c
    ${SDK_BASE}/modules/FreeRTOS/FreeRTOS/Source/queue.c
    ${SDK_BASE}/modules/FreeRTOS/FreeRTOS/Source/stream_buffer.c
    ${SDK_BASE}/modules/FreeRTOS/FreeRTOS/Source/timers.c

)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${SDK_BASE}/modules/FreeRTOS/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1
    ${SDK_BASE}/modules/FreeRTOS/FreeRTOS/Source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)