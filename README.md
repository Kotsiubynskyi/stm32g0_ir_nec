The repository demonstrates Input Capture mode of ***STM32G0F6P6*** timer on example of Infra-Red NEC protocol

## How to build and flash (for Ubuntu)

1. Install needed ARM development toolchain and other utilities:
    ```bash
    apt update; apt install -y cmake git usbutils ninja-build gcc-arm-none-eabi stlink-tools
    ```
1. Clone the project:
    ```bash
    git clone https://github.com/Kotsiubynskyi/stm32g0_ir_nec.git
    ```
1. Generate build files:
    ```bash
    cd stm32g0_ir_nec
    mkdir build
    cd build
    cmake .. -G Ninja
    ```
1. Build project from ***build*** folder:
    ```bash
    ninja
    ```
1. Insert ST-LINK programmer USB stick and make sure it's visible by OS:
    ```bash
    lsusb|grep STMicroelectronics
    ```
1. Upload built firmware to MCU:
    ```bash
    st-flash erase && st-flash --reset write timer_ic.bin 0x8000000
    ```
