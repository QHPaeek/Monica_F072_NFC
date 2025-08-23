################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Card_Read.c \
../Core/Src/LED.c \
../Core/Src/bsp.c \
../Core/Src/custom_bus.c \
../Core/Src/dma.c \
../Core/Src/flash.c \
../Core/Src/gpio.c \
../Core/Src/main.c \
../Core/Src/mcc.c \
../Core/Src/mcc_crypto.c \
../Core/Src/mcc_crypto_card.c \
../Core/Src/mcc_crypto_reader.c \
../Core/Src/mcc_macros.c \
../Core/Src/mcc_raw_request.c \
../Core/Src/mode_manager.c \
../Core/Src/mode_sega_serial.c \
../Core/Src/mode_spice_api.c \
../Core/Src/stm32f0xx_hal_msp.c \
../Core/Src/stm32f0xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f0xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c \
../Core/Src/usb.c 

OBJS += \
./Core/Src/Card_Read.o \
./Core/Src/LED.o \
./Core/Src/bsp.o \
./Core/Src/custom_bus.o \
./Core/Src/dma.o \
./Core/Src/flash.o \
./Core/Src/gpio.o \
./Core/Src/main.o \
./Core/Src/mcc.o \
./Core/Src/mcc_crypto.o \
./Core/Src/mcc_crypto_card.o \
./Core/Src/mcc_crypto_reader.o \
./Core/Src/mcc_macros.o \
./Core/Src/mcc_raw_request.o \
./Core/Src/mode_manager.o \
./Core/Src/mode_sega_serial.o \
./Core/Src/mode_spice_api.o \
./Core/Src/stm32f0xx_hal_msp.o \
./Core/Src/stm32f0xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f0xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o \
./Core/Src/usb.o 

C_DEPS += \
./Core/Src/Card_Read.d \
./Core/Src/LED.d \
./Core/Src/bsp.d \
./Core/Src/custom_bus.d \
./Core/Src/dma.d \
./Core/Src/flash.d \
./Core/Src/gpio.d \
./Core/Src/main.d \
./Core/Src/mcc.d \
./Core/Src/mcc_crypto.d \
./Core/Src/mcc_crypto_card.d \
./Core/Src/mcc_crypto_reader.d \
./Core/Src/mcc_macros.d \
./Core/Src/mcc_raw_request.d \
./Core/Src/mode_manager.d \
./Core/Src/mode_sega_serial.d \
./Core/Src/mode_spice_api.d \
./Core/Src/stm32f0xx_hal_msp.d \
./Core/Src/stm32f0xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f0xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d \
./Core/Src/usb.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F072xB -DST25R3916 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../X-CUBE-NFC6/Target -I../Drivers/BSP/Components/ST25R3916 -I../Drivers/BSP/NFC06A1 -I../Middlewares/ST/rfal/Inc -I../Middlewares/ST/rfal/Src -I../Composite -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/COMPOSITE/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_CUSTOM/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Core/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/CDC_ACM/Inc -I../X-CUBE-NFC6/App -I../Drivers/BSP/custom -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/Card_Read.cyclo ./Core/Src/Card_Read.d ./Core/Src/Card_Read.o ./Core/Src/Card_Read.su ./Core/Src/LED.cyclo ./Core/Src/LED.d ./Core/Src/LED.o ./Core/Src/LED.su ./Core/Src/bsp.cyclo ./Core/Src/bsp.d ./Core/Src/bsp.o ./Core/Src/bsp.su ./Core/Src/custom_bus.cyclo ./Core/Src/custom_bus.d ./Core/Src/custom_bus.o ./Core/Src/custom_bus.su ./Core/Src/dma.cyclo ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/flash.cyclo ./Core/Src/flash.d ./Core/Src/flash.o ./Core/Src/flash.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/mcc.cyclo ./Core/Src/mcc.d ./Core/Src/mcc.o ./Core/Src/mcc.su ./Core/Src/mcc_crypto.cyclo ./Core/Src/mcc_crypto.d ./Core/Src/mcc_crypto.o ./Core/Src/mcc_crypto.su ./Core/Src/mcc_crypto_card.cyclo ./Core/Src/mcc_crypto_card.d ./Core/Src/mcc_crypto_card.o ./Core/Src/mcc_crypto_card.su ./Core/Src/mcc_crypto_reader.cyclo ./Core/Src/mcc_crypto_reader.d ./Core/Src/mcc_crypto_reader.o ./Core/Src/mcc_crypto_reader.su ./Core/Src/mcc_macros.cyclo ./Core/Src/mcc_macros.d ./Core/Src/mcc_macros.o ./Core/Src/mcc_macros.su ./Core/Src/mcc_raw_request.cyclo ./Core/Src/mcc_raw_request.d ./Core/Src/mcc_raw_request.o ./Core/Src/mcc_raw_request.su ./Core/Src/mode_manager.cyclo ./Core/Src/mode_manager.d ./Core/Src/mode_manager.o ./Core/Src/mode_manager.su ./Core/Src/mode_sega_serial.cyclo ./Core/Src/mode_sega_serial.d ./Core/Src/mode_sega_serial.o ./Core/Src/mode_sega_serial.su ./Core/Src/mode_spice_api.cyclo ./Core/Src/mode_spice_api.d ./Core/Src/mode_spice_api.o ./Core/Src/mode_spice_api.su ./Core/Src/stm32f0xx_hal_msp.cyclo ./Core/Src/stm32f0xx_hal_msp.d ./Core/Src/stm32f0xx_hal_msp.o ./Core/Src/stm32f0xx_hal_msp.su ./Core/Src/stm32f0xx_it.cyclo ./Core/Src/stm32f0xx_it.d ./Core/Src/stm32f0xx_it.o ./Core/Src/stm32f0xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f0xx.cyclo ./Core/Src/system_stm32f0xx.d ./Core/Src/system_stm32f0xx.o ./Core/Src/system_stm32f0xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/usb.cyclo ./Core/Src/usb.d ./Core/Src/usb.o ./Core/Src/usb.su

.PHONY: clean-Core-2f-Src

