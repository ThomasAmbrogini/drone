add_library(arm_cmsis INTERFACE)
target_include_directories(arm_cmsis
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/cmsis
)
