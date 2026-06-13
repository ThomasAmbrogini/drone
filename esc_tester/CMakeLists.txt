set(ESC_TESTER_NAME esc_tester)
add_executable(${ESC_TESTER_NAME}.elf main.cc)

target_link_libraries(${ESC_TESTER_NAME}.elf
    PRIVATE
        drone_options
        drone_warnings

    PRIVATE
        coco
)

target_link_options(${ESC_TESTER_NAME}.elf
    PRIVATE
        LINKER:-Map=${ESC_TESTER_NAME}.map
)

if (DRONE_LINKER_FILE_PATH)
    target_link_options(${ESC_TESTER_NAME}.elf
        PRIVATE
            -T${DRONE_LINKER_FILE_PATH}
    )
endif()
