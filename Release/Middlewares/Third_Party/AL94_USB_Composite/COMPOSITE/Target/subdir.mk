################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target/usbd_conf.c 

OBJS += \
./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target/usbd_conf.o 

C_DEPS += \
./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target/usbd_conf.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target/%.o Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target/%.su Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target/%.cyclo: ../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target/%.c Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F072xB -DST25R3916 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../X-CUBE-NFC6/Target -I../Drivers/BSP/Components/ST25R3916 -I../Drivers/BSP/NFC06A1 -I../Middlewares/ST/rfal/Inc -I../Middlewares/ST/rfal/Src -I../Composite -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/COMPOSITE/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_CUSTOM/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Core/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/CDC_ACM/Inc -I../X-CUBE-NFC6/App -I../Drivers/BSP/custom -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-AL94_USB_Composite-2f-COMPOSITE-2f-Target

clean-Middlewares-2f-Third_Party-2f-AL94_USB_Composite-2f-COMPOSITE-2f-Target:
	-$(RM) ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target/usbd_conf.cyclo ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target/usbd_conf.d ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target/usbd_conf.o ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target/usbd_conf.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-AL94_USB_Composite-2f-COMPOSITE-2f-Target

