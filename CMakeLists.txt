cmake_minimum_required(VERSION 4.0)

project(drone
    VERSION 0.1.1
    LANGUAGES C CXX ASM
)

set(CMAKE_C_STANDARD          23)
set(CMAKE_CXX_STANDARD        26)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DRONE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DRONE_C_FLAGS} ${DRONE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${DRONE_EXE_LINKER_FLAGS}")

add_library(drone_warnings INTERFACE)
target_compile_options(drone_warnings
    INTERFACE
        -Wall
        -Wextra
)

add_library(drone_options INTERFACE)
target_compile_options(drone_options
    INTERFACE
        -ffunction-sections
        -fdata-sections

        $<$<CONFIG:Debug>:-Og -g3>
        $<$<CONFIG:Release>:-O3>
)

target_link_options(drone_options
    INTERFACE
        LINKER:--gc-sections
)

#TODO: are we sure this is the correct way? Even with multi config?
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

add_subdirectory(coco)
add_subdirectory(esc_tester)

