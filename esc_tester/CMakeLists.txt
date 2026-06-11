add_executable(esc_tester.elf main.cc)

target_link_libraries(esc_tester.elf
    PRIVATE
        drone_options
        drone_warnings

    PRIVATE
        coco
)

target_link_options(esc_tester.elf
    PRIVATE
        -T${CMAKE_CURRENT_SOURCE_DIR}/linker/STM32F446RETx_FLASH.ld
        -Wl,-Map=esc_tester.map
)
