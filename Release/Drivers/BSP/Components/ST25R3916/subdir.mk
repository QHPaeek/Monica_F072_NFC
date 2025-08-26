################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/ST25R3916/rfal_rfst25r3916.c \
../Drivers/BSP/Components/ST25R3916/st25r3916.c \
../Drivers/BSP/Components/ST25R3916/st25r3916_aat.c \
../Drivers/BSP/Components/ST25R3916/st25r3916_com.c \
../Drivers/BSP/Components/ST25R3916/st25r3916_irq.c \
../Drivers/BSP/Components/ST25R3916/st25r3916_led.c \
../Drivers/BSP/Components/ST25R3916/timer.c 

OBJS += \
./Drivers/BSP/Components/ST25R3916/rfal_rfst25r3916.o \
./Drivers/BSP/Components/ST25R3916/st25r3916.o \
./Drivers/BSP/Components/ST25R3916/st25r3916_aat.o \
./Drivers/BSP/Components/ST25R3916/st25r3916_com.o \
./Drivers/BSP/Components/ST25R3916/st25r3916_irq.o \
./Drivers/BSP/Components/ST25R3916/st25r3916_led.o \
./Drivers/BSP/Components/ST25R3916/timer.o 

C_DEPS += \
./Drivers/BSP/Components/ST25R3916/rfal_rfst25r3916.d \
./Drivers/BSP/Components/ST25R3916/st25r3916.d \
./Drivers/BSP/Components/ST25R3916/st25r3916_aat.d \
./Drivers/BSP/Components/ST25R3916/st25r3916_com.d \
./Drivers/BSP/Components/ST25R3916/st25r3916_irq.d \
./Drivers/BSP/Components/ST25R3916/st25r3916_led.d \
./Drivers/BSP/Components/ST25R3916/timer.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/ST25R3916/%.o Drivers/BSP/Components/ST25R3916/%.su Drivers/BSP/Components/ST25R3916/%.cyclo: ../Drivers/BSP/Components/ST25R3916/%.c Drivers/BSP/Components/ST25R3916/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F072xB -DST25R3916 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../X-CUBE-NFC6/Target -I../Drivers/BSP/Components/ST25R3916 -I../Drivers/BSP/NFC06A1 -I../Middlewares/ST/rfal/Inc -I../Middlewares/ST/rfal/Src -I../Composite -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/COMPOSITE/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_CUSTOM/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Core/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/CDC_ACM/Inc -I../X-CUBE-NFC6/App -I../Drivers/BSP/custom -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components-2f-ST25R3916

clean-Drivers-2f-BSP-2f-Components-2f-ST25R3916:
	-$(RM) ./Drivers/BSP/Components/ST25R3916/rfal_rfst25r3916.cyclo ./Drivers/BSP/Components/ST25R3916/rfal_rfst25r3916.d ./Drivers/BSP/Components/ST25R3916/rfal_rfst25r3916.o ./Drivers/BSP/Components/ST25R3916/rfal_rfst25r3916.su ./Drivers/BSP/Components/ST25R3916/st25r3916.cyclo ./Drivers/BSP/Components/ST25R3916/st25r3916.d ./Drivers/BSP/Components/ST25R3916/st25r3916.o ./Drivers/BSP/Components/ST25R3916/st25r3916.su ./Drivers/BSP/Components/ST25R3916/st25r3916_aat.cyclo ./Drivers/BSP/Components/ST25R3916/st25r3916_aat.d ./Drivers/BSP/Components/ST25R3916/st25r3916_aat.o ./Drivers/BSP/Components/ST25R3916/st25r3916_aat.su ./Drivers/BSP/Components/ST25R3916/st25r3916_com.cyclo ./Drivers/BSP/Components/ST25R3916/st25r3916_com.d ./Drivers/BSP/Components/ST25R3916/st25r3916_com.o ./Drivers/BSP/Components/ST25R3916/st25r3916_com.su ./Drivers/BSP/Components/ST25R3916/st25r3916_irq.cyclo ./Drivers/BSP/Components/ST25R3916/st25r3916_irq.d ./Drivers/BSP/Components/ST25R3916/st25r3916_irq.o ./Drivers/BSP/Components/ST25R3916/st25r3916_irq.su ./Drivers/BSP/Components/ST25R3916/st25r3916_led.cyclo ./Drivers/BSP/Components/ST25R3916/st25r3916_led.d ./Drivers/BSP/Components/ST25R3916/st25r3916_led.o ./Drivers/BSP/Components/ST25R3916/st25r3916_led.su ./Drivers/BSP/Components/ST25R3916/timer.cyclo ./Drivers/BSP/Components/ST25R3916/timer.d ./Drivers/BSP/Components/ST25R3916/timer.o ./Drivers/BSP/Components/ST25R3916/timer.su

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-ST25R3916

