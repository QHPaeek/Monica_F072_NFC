################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../X-CUBE-NFC6/Target/analogConfigTbl_NFC06A1.c \
../X-CUBE-NFC6/Target/demo_ce.c \
../X-CUBE-NFC6/Target/demo_polling.c \
../X-CUBE-NFC6/Target/logger.c \
../X-CUBE-NFC6/Target/nfc_conf.c 

OBJS += \
./X-CUBE-NFC6/Target/analogConfigTbl_NFC06A1.o \
./X-CUBE-NFC6/Target/demo_ce.o \
./X-CUBE-NFC6/Target/demo_polling.o \
./X-CUBE-NFC6/Target/logger.o \
./X-CUBE-NFC6/Target/nfc_conf.o 

C_DEPS += \
./X-CUBE-NFC6/Target/analogConfigTbl_NFC06A1.d \
./X-CUBE-NFC6/Target/demo_ce.d \
./X-CUBE-NFC6/Target/demo_polling.d \
./X-CUBE-NFC6/Target/logger.d \
./X-CUBE-NFC6/Target/nfc_conf.d 


# Each subdirectory must supply rules for building sources it contributes
X-CUBE-NFC6/Target/%.o X-CUBE-NFC6/Target/%.su X-CUBE-NFC6/Target/%.cyclo: ../X-CUBE-NFC6/Target/%.c X-CUBE-NFC6/Target/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F072xB -DST25R3916 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../X-CUBE-NFC6/Target -I../Drivers/BSP/Components/ST25R3916 -I../Drivers/BSP/NFC06A1 -I../Middlewares/ST/rfal/Inc -I../Middlewares/ST/rfal/Src -I../Composite -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/COMPOSITE/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_CUSTOM/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Core/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/CDC_ACM/Inc -I../X-CUBE-NFC6/App -I../Drivers/BSP/custom -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-X-2d-CUBE-2d-NFC6-2f-Target

clean-X-2d-CUBE-2d-NFC6-2f-Target:
	-$(RM) ./X-CUBE-NFC6/Target/analogConfigTbl_NFC06A1.cyclo ./X-CUBE-NFC6/Target/analogConfigTbl_NFC06A1.d ./X-CUBE-NFC6/Target/analogConfigTbl_NFC06A1.o ./X-CUBE-NFC6/Target/analogConfigTbl_NFC06A1.su ./X-CUBE-NFC6/Target/demo_ce.cyclo ./X-CUBE-NFC6/Target/demo_ce.d ./X-CUBE-NFC6/Target/demo_ce.o ./X-CUBE-NFC6/Target/demo_ce.su ./X-CUBE-NFC6/Target/demo_polling.cyclo ./X-CUBE-NFC6/Target/demo_polling.d ./X-CUBE-NFC6/Target/demo_polling.o ./X-CUBE-NFC6/Target/demo_polling.su ./X-CUBE-NFC6/Target/logger.cyclo ./X-CUBE-NFC6/Target/logger.d ./X-CUBE-NFC6/Target/logger.o ./X-CUBE-NFC6/Target/logger.su ./X-CUBE-NFC6/Target/nfc_conf.cyclo ./X-CUBE-NFC6/Target/nfc_conf.d ./X-CUBE-NFC6/Target/nfc_conf.o ./X-CUBE-NFC6/Target/nfc_conf.su

.PHONY: clean-X-2d-CUBE-2d-NFC6-2f-Target

