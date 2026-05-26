cmake_minimum_required(VERSION 4.0)

project(esc_tester
        LANGUAGES C CXX ASM
)

set(CMAKE_C_STANDARD   11)
set(CMAKE_CXX_STANDARD 17)

set(MCU_DEFINES
    STM32F446xx
    USE_FULL_LL_DRIVER
)

set(COMMON_FLAGS
    -Wall
    -Wextra
    -fdata-sections
    -ffunction-sections
    -fno-exceptions
)

set(INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/third_party/CMSIS
    ${CMAKE_SOURCE_DIR}/third_party/LL/Inc
)

set(APP_SOURCES
    main.cc
)

set(CMSIS_SOURCES
    third_party/CMSIS/system_stm32f4xx.c
    third_party/CMSIS/startup_stm32f446xx.s
)

set(LL_SOURCES
    third_party/LL/Src/stm32f4xx_ll_gpio.c
    third_party/LL/Src/stm32f4xx_ll_tim.c
    third_party/LL/Src/stm32f4xx_ll_rcc.c
    third_party/LL/Src/stm32f4xx_ll_utils.c
    third_party/LL/Src/stm32f4xx_ll_pwr.c
)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/linker/STM32F446RETx_FLASH.ld)

set(LINKER_FLAGS
    -T${LINKER_SCRIPT}
    -Wl,--gc-sections
    -Wl,-Map=${PROJECT_NAME}.map
    -Wl,--print-memory-usage
    --specs=nano.specs
    --specs=nosys.specs
)

add_executable(${PROJECT_NAME}.elf
    ${APP_SOURCES}
    ${CMSIS_SOURCES}
    ${LL_SOURCES}
)

target_compile_definitions(${PROJECT_NAME}.elf PRIVATE ${MCU_DEFINES})
target_compile_options(${PROJECT_NAME}.elf     PRIVATE ${COMMON_FLAGS})
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${INCLUDE_DIRS})
target_link_options(${PROJECT_NAME}.elf        PRIVATE ${LINKER_FLAGS})

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${PROJECT_NAME}.elf>
    COMMENT "Generating ${PROJECT_NAME}.bin"
)

