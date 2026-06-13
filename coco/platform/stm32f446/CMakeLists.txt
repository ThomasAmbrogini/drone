add_subdirectory(../../third_party ${CMAKE_CURRENT_BINARY_DIR}/third_party)

target_sources(coco
    PRIVATE
        hal/hal_pwm_stm32.cc
        os/stm32_platform.cc
        os/stm32_sleep.cc
)

target_link_libraries(coco
    PUBLIC
        stm32f4_vendor
)

