cmake_minimum_required(VERSION 3.10)

# Define the project name
project(test_f407_bootload C ASM)

# 指定 GCC 编译器
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc")
# Specify the target
set(TARGET test_f407_bootload)

# Specify the optimization level
set(OPT "-Og")

# Define the build directory
set(BUILD_DIR ${CMAKE_BINARY_DIR}/build)

# C sources
set(C_SOURCES
    Core/Src/main.c
    Core/Src/gpio.c
    Core/Src/stm32f4xx_it.c
    Core/Src/stm32f4xx_hal_msp.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
    Core/Src/system_stm32f4xx.c
    Core/Src/sysmem.c
    Core/Src/syscalls.c
    Core/Src/usart.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
)

# ASM sources
set(ASM_SOURCES
    startup_stm32f407xx.s
)

# Include directories
include_directories(
    Core/Inc
    Drivers/STM32F4xx_HAL_Driver/Inc
    Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/CMSIS/Include
)

# Define the MCU flags
set(CPU_FLAGS "-mcpu=cortex-m4")
set(FPU_FLAGS "-mfpu=fpv4-sp-d16")
set(FLOAT_ABI_FLAGS "-mfloat-abi=hard")
set(MCU_FLAGS "${CPU_FLAGS} -mthumb ${FPU_FLAGS} ${FLOAT_ABI_FLAGS}")

# Set compiler flags
set(CMAKE_C_FLAGS "${MCU_FLAGS} ${C_DEFS} ${C_INCLUDES} ${OPT} -Wall -fdata-sections -ffunction-sections")
set(CMAKE_ASM_FLAGS "${MCU_FLAGS} ${AS_DEFS} ${AS_INCLUDES} ${OPT} -Wall -fdata-sections -ffunction-sections")

# Add debug flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -gdwarf-2")
endif()

# Linker script
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F407VETx_FLASH.ld)

# Define the executable
add_executable(${TARGET}.elf ${C_SOURCES} ${ASM_SOURCES})

# Specify the linker flags
target_link_options(${TARGET}.elf PRIVATE "-T${LINKER_SCRIPT} -specs=nano.specs -Wl,-Map=${BUILD_DIR}/${TARGET}.map,--cref -Wl,--gc-sections")

# Add libraries
target_link_libraries(${TARGET}.elf c m nosys)

# Create bin and hex files
add_custom_command(TARGET ${TARGET}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${TARGET}.elf> ${BUILD_DIR}/${TARGET}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary -S $<TARGET_FILE:${TARGET}.elf> ${BUILD_DIR}/${TARGET}.bin
)

# Clean up
# add_custom_target(clean COMMAND ${CMAKE_COMMAND} -E remove_directory ${BUILD_DIR})
# Clean up
add_custom_target(clean_build COMMAND ${CMAKE_COMMAND} -E remove_directory ${BUILD_DIR})
