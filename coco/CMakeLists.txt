cmake_minimum_required(VERSION 4.0)

project(coco
    VERSION 0.1.0
    LANGUAGES C CXX ASM
)

set(COCO_PLATFORM "" CACHE STRING "Target platform (e.g. stm32f446)")
if(NOT COCO_PLATFORM)
    message(FATAL_ERROR "COCO_PLATFORM must be set (e.g. stm32f446)")
endif()

add_library(coco_definitions INTERFACE)
target_compile_definitions(coco_definitions
    INTERFACE
        $<$<CONFIG:Debug>:DEBUG>
)

add_library(coco STATIC)

target_link_libraries(coco
    PUBLIC
        coco_definitions

    PRIVATE
        #TODO: maybe i can create my own options for this project in here.
        drone_warnings
        drone_options
)

target_include_directories(coco
    PUBLIC
        include
)

add_subdirectory(platform/${COCO_PLATFORM})

