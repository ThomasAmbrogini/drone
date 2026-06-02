cmake_minimum_required(VERSION 4.0)

project(drone
    VERSION 0.1.1
    LANGUAGES C CXX ASM
)

set(CMAKE_C_STANDARD          23)
set(CMAKE_CXX_STANDARD        26)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#TODO: are we sure this is the correct way? Even with multi config?
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

#TODO: I do not want them in here.
add_library(drone_compile_options INTERFACE)
target_compile_options(drone_compile_options
    INTERFACE
        -Wall
        -Wextra
        -ffreestanding
        -ffunction-sections
        -fdata-sections
        $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
        $<$<CONFIG:Debug>:-Og -g3>
        $<$<CONFIG:Release>:-O2>
)
target_link_options(drone_compile_options
    INTERFACE
        -Wl,--gc-sections
)

add_subdirectory(coco)
add_subdirectory(esc_tester)

