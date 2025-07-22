################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/rfal/Src/rfal_analogConfig.c \
../Middlewares/ST/rfal/Src/rfal_cd.c \
../Middlewares/ST/rfal/Src/rfal_crc.c \
../Middlewares/ST/rfal/Src/rfal_dpo.c \
../Middlewares/ST/rfal/Src/rfal_iso15693_2.c \
../Middlewares/ST/rfal/Src/rfal_isoDep.c \
../Middlewares/ST/rfal/Src/rfal_nfc.c \
../Middlewares/ST/rfal/Src/rfal_nfcDep.c \
../Middlewares/ST/rfal/Src/rfal_nfca.c \
../Middlewares/ST/rfal/Src/rfal_nfcb.c \
../Middlewares/ST/rfal/Src/rfal_nfcf.c \
../Middlewares/ST/rfal/Src/rfal_nfcv.c \
../Middlewares/ST/rfal/Src/rfal_st25tb.c \
../Middlewares/ST/rfal/Src/rfal_st25xv.c \
../Middlewares/ST/rfal/Src/rfal_t1t.c \
../Middlewares/ST/rfal/Src/rfal_t2t.c \
../Middlewares/ST/rfal/Src/rfal_t4t.c 

OBJS += \
./Middlewares/ST/rfal/Src/rfal_analogConfig.o \
./Middlewares/ST/rfal/Src/rfal_cd.o \
./Middlewares/ST/rfal/Src/rfal_crc.o \
./Middlewares/ST/rfal/Src/rfal_dpo.o \
./Middlewares/ST/rfal/Src/rfal_iso15693_2.o \
./Middlewares/ST/rfal/Src/rfal_isoDep.o \
./Middlewares/ST/rfal/Src/rfal_nfc.o \
./Middlewares/ST/rfal/Src/rfal_nfcDep.o \
./Middlewares/ST/rfal/Src/rfal_nfca.o \
./Middlewares/ST/rfal/Src/rfal_nfcb.o \
./Middlewares/ST/rfal/Src/rfal_nfcf.o \
./Middlewares/ST/rfal/Src/rfal_nfcv.o \
./Middlewares/ST/rfal/Src/rfal_st25tb.o \
./Middlewares/ST/rfal/Src/rfal_st25xv.o \
./Middlewares/ST/rfal/Src/rfal_t1t.o \
./Middlewares/ST/rfal/Src/rfal_t2t.o \
./Middlewares/ST/rfal/Src/rfal_t4t.o 

C_DEPS += \
./Middlewares/ST/rfal/Src/rfal_analogConfig.d \
./Middlewares/ST/rfal/Src/rfal_cd.d \
./Middlewares/ST/rfal/Src/rfal_crc.d \
./Middlewares/ST/rfal/Src/rfal_dpo.d \
./Middlewares/ST/rfal/Src/rfal_iso15693_2.d \
./Middlewares/ST/rfal/Src/rfal_isoDep.d \
./Middlewares/ST/rfal/Src/rfal_nfc.d \
./Middlewares/ST/rfal/Src/rfal_nfcDep.d \
./Middlewares/ST/rfal/Src/rfal_nfca.d \
./Middlewares/ST/rfal/Src/rfal_nfcb.d \
./Middlewares/ST/rfal/Src/rfal_nfcf.d \
./Middlewares/ST/rfal/Src/rfal_nfcv.d \
./Middlewares/ST/rfal/Src/rfal_st25tb.d \
./Middlewares/ST/rfal/Src/rfal_st25xv.d \
./Middlewares/ST/rfal/Src/rfal_t1t.d \
./Middlewares/ST/rfal/Src/rfal_t2t.d \
./Middlewares/ST/rfal/Src/rfal_t4t.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/rfal/Src/%.o Middlewares/ST/rfal/Src/%.su Middlewares/ST/rfal/Src/%.cyclo: ../Middlewares/ST/rfal/Src/%.c Middlewares/ST/rfal/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -DST25R3916 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../X-CUBE-NFC6/Target -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -I../Drivers/BSP/Components/ST25R3916 -I../Drivers/BSP/NFC06A1 -I../Middlewares/ST/rfal/Inc -I../Middlewares/ST/rfal/Src -I../Composite -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/COMPOSITE/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_CUSTOM/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Core/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/CDC_ACM/Inc -I../X-CUBE-NFC6/App -I../Drivers/BSP/custom -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-rfal-2f-Src

clean-Middlewares-2f-ST-2f-rfal-2f-Src:
	-$(RM) ./Middlewares/ST/rfal/Src/rfal_analogConfig.cyclo ./Middlewares/ST/rfal/Src/rfal_analogConfig.d ./Middlewares/ST/rfal/Src/rfal_analogConfig.o ./Middlewares/ST/rfal/Src/rfal_analogConfig.su ./Middlewares/ST/rfal/Src/rfal_cd.cyclo ./Middlewares/ST/rfal/Src/rfal_cd.d ./Middlewares/ST/rfal/Src/rfal_cd.o ./Middlewares/ST/rfal/Src/rfal_cd.su ./Middlewares/ST/rfal/Src/rfal_crc.cyclo ./Middlewares/ST/rfal/Src/rfal_crc.d ./Middlewares/ST/rfal/Src/rfal_crc.o ./Middlewares/ST/rfal/Src/rfal_crc.su ./Middlewares/ST/rfal/Src/rfal_dpo.cyclo ./Middlewares/ST/rfal/Src/rfal_dpo.d ./Middlewares/ST/rfal/Src/rfal_dpo.o ./Middlewares/ST/rfal/Src/rfal_dpo.su ./Middlewares/ST/rfal/Src/rfal_iso15693_2.cyclo ./Middlewares/ST/rfal/Src/rfal_iso15693_2.d ./Middlewares/ST/rfal/Src/rfal_iso15693_2.o ./Middlewares/ST/rfal/Src/rfal_iso15693_2.su ./Middlewares/ST/rfal/Src/rfal_isoDep.cyclo ./Middlewares/ST/rfal/Src/rfal_isoDep.d ./Middlewares/ST/rfal/Src/rfal_isoDep.o ./Middlewares/ST/rfal/Src/rfal_isoDep.su ./Middlewares/ST/rfal/Src/rfal_nfc.cyclo ./Middlewares/ST/rfal/Src/rfal_nfc.d ./Middlewares/ST/rfal/Src/rfal_nfc.o ./Middlewares/ST/rfal/Src/rfal_nfc.su ./Middlewares/ST/rfal/Src/rfal_nfcDep.cyclo ./Middlewares/ST/rfal/Src/rfal_nfcDep.d ./Middlewares/ST/rfal/Src/rfal_nfcDep.o ./Middlewares/ST/rfal/Src/rfal_nfcDep.su ./Middlewares/ST/rfal/Src/rfal_nfca.cyclo ./Middlewares/ST/rfal/Src/rfal_nfca.d ./Middlewares/ST/rfal/Src/rfal_nfca.o ./Middlewares/ST/rfal/Src/rfal_nfca.su ./Middlewares/ST/rfal/Src/rfal_nfcb.cyclo ./Middlewares/ST/rfal/Src/rfal_nfcb.d ./Middlewares/ST/rfal/Src/rfal_nfcb.o ./Middlewares/ST/rfal/Src/rfal_nfcb.su ./Middlewares/ST/rfal/Src/rfal_nfcf.cyclo ./Middlewares/ST/rfal/Src/rfal_nfcf.d ./Middlewares/ST/rfal/Src/rfal_nfcf.o ./Middlewares/ST/rfal/Src/rfal_nfcf.su ./Middlewares/ST/rfal/Src/rfal_nfcv.cyclo ./Middlewares/ST/rfal/Src/rfal_nfcv.d ./Middlewares/ST/rfal/Src/rfal_nfcv.o ./Middlewares/ST/rfal/Src/rfal_nfcv.su ./Middlewares/ST/rfal/Src/rfal_st25tb.cyclo ./Middlewares/ST/rfal/Src/rfal_st25tb.d ./Middlewares/ST/rfal/Src/rfal_st25tb.o ./Middlewares/ST/rfal/Src/rfal_st25tb.su ./Middlewares/ST/rfal/Src/rfal_st25xv.cyclo ./Middlewares/ST/rfal/Src/rfal_st25xv.d ./Middlewares/ST/rfal/Src/rfal_st25xv.o ./Middlewares/ST/rfal/Src/rfal_st25xv.su ./Middlewares/ST/rfal/Src/rfal_t1t.cyclo ./Middlewares/ST/rfal/Src/rfal_t1t.d ./Middlewares/ST/rfal/Src/rfal_t1t.o ./Middlewares/ST/rfal/Src/rfal_t1t.su ./Middlewares/ST/rfal/Src/rfal_t2t.cyclo ./Middlewares/ST/rfal/Src/rfal_t2t.d ./Middlewares/ST/rfal/Src/rfal_t2t.o ./Middlewares/ST/rfal/Src/rfal_t2t.su ./Middlewares/ST/rfal/Src/rfal_t4t.cyclo ./Middlewares/ST/rfal/Src/rfal_t4t.d ./Middlewares/ST/rfal/Src/rfal_t4t.o ./Middlewares/ST/rfal/Src/rfal_t4t.su

.PHONY: clean-Middlewares-2f-ST-2f-rfal-2f-Src

