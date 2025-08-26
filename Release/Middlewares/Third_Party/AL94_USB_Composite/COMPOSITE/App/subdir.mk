################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usb_device.c \
../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_cdc_acm_if.c \
../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_desc.c \
../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_hid_custom_if.c 

OBJS += \
./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usb_device.o \
./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_cdc_acm_if.o \
./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_desc.o \
./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_hid_custom_if.o 

C_DEPS += \
./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usb_device.d \
./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_cdc_acm_if.d \
./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_desc.d \
./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_hid_custom_if.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/%.o Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/%.su Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/%.cyclo: ../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/%.c Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F072xB -DST25R3916 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../X-CUBE-NFC6/Target -I../Drivers/BSP/Components/ST25R3916 -I../Drivers/BSP/NFC06A1 -I../Middlewares/ST/rfal/Inc -I../Middlewares/ST/rfal/Src -I../Composite -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/COMPOSITE/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_CUSTOM/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Core/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/CDC_ACM/Inc -I../X-CUBE-NFC6/App -I../Drivers/BSP/custom -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-AL94_USB_Composite-2f-COMPOSITE-2f-App

clean-Middlewares-2f-Third_Party-2f-AL94_USB_Composite-2f-COMPOSITE-2f-App:
	-$(RM) ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usb_device.cyclo ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usb_device.d ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usb_device.o ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usb_device.su ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_cdc_acm_if.cyclo ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_cdc_acm_if.d ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_cdc_acm_if.o ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_cdc_acm_if.su ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_desc.cyclo ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_desc.d ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_desc.o ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_desc.su ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_hid_custom_if.cyclo ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_hid_custom_if.d ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_hid_custom_if.o ./Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App/usbd_hid_custom_if.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-AL94_USB_Composite-2f-COMPOSITE-2f-App

