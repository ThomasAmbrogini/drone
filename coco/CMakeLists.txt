cmake_minimum_required(VERSION 4.0)

project(coco
    VERSION 0.1.0
    LANGUAGES C CXX ASM
)

set(COCO_PLATFORM "" CACHE STRING "Target platform (e.g. stm32f446)")
if(NOT COCO_PLATFORM)
    message(FATAL_ERROR "COCO_PLATFORM must be set (e.g. stm32f446)")
endif()

add_library(coco STATIC)

target_include_directories(coco
    PUBLIC
        include
)

target_compile_options(coco
    PRIVATE
        -ffreestanding
        $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
)

add_subdirectory(third_party)
add_subdirectory(platform/${COCO_PLATFORM})
