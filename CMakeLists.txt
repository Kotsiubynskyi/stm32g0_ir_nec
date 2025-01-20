cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE "STM32G0_toolchain.cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(timer_ic LANGUAGES C CXX ASM)

# Source files
add_executable(${PROJECT_NAME}
  src/main.c
  src/stm32g0xx_it.c
  src/system_stm32g0xx.c
  src/startup_stm32g030xx.s
  src/hal/stm32g0xx_hal.c
  src/hal/stm32g0xx_hal_tim.c
  src/hal/stm32g0xx_hal_tim_ex.c
  src/hal/stm32g0xx_hal_gpio.c
  src/hal/stm32g0xx_hal_cortex.c
)

# Directories with headers
target_include_directories(${PROJECT_NAME} PRIVATE
  include
  include/examples
  include/hal
  include/hal/Legacy
  include/cmsis
)

# Define compile options
target_compile_options(${PROJECT_NAME} PRIVATE
  -mcpu=cortex-m0
  -mthumb
  -g
  # -Os
  -ffunction-sections # Place functions in separate sections
  -fdata-sections # Place data in separate sections
  -fno-exceptions # Disable exceptions for bare-metal code
)

# Linker options
target_link_options(${PROJECT_NAME} PRIVATE
  -mcpu=cortex-m0
  -mthumb
  -Wl,--start-group
  -Wl,--end-group
  -static
)

# Define defines here
target_compile_definitions(${PROJECT_NAME} PRIVATE
  STM32G030xx # Define your G0 series MCU
)

# Convert .elf firmware to .bin after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME} ${PROJECT_NAME}.bin
)

# Print firmware Flash and RAM usage
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}
)

