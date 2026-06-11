add_library(stm32f4_vendor STATIC)
target_sources(stm32f4_vendor
    PRIVATE
        cmsis/system_stm32f4xx.c
        cmsis/startup_stm32f446xx.s
        ll/Src/stm32f4xx_ll_gpio.c
        ll/Src/stm32f4xx_ll_tim.c
        ll/Src/stm32f4xx_ll_rcc.c
        ll/Src/stm32f4xx_ll_utils.c
        ll/Src/stm32f4xx_ll_pwr.c
)

target_compile_definitions(stm32f4_vendor
    PUBLIC
        STM32F446xx
        USE_FULL_LL_DRIVER
)

target_include_directories(stm32f4_vendor
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/cmsis
        ${CMAKE_CURRENT_SOURCE_DIR}/ll/Inc
)

target_link_libraries(stm32f4_vendor
    PUBLIC
        arm_cmsis
)

target_compile_options(stm32f4_vendor
    PRIVATE
        -ffreestanding
        -ffunction-sections
        -fdata-sections
)

