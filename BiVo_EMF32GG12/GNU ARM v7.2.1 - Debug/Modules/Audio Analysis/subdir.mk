################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Modules/Audio\ Analysis/audio_analysis.c 

OBJS += \
./Modules/Audio\ Analysis/audio_analysis.o 

C_DEPS += \
./Modules/Audio\ Analysis/audio_analysis.d 


# Each subdirectory must supply rules for building sources it contributes
Modules/Audio\ Analysis/audio_analysis.o: ../Modules/Audio\ Analysis/audio_analysis.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DARM_MATH_CM4=1' '-DDEBUG_EFM=1' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DEFM32GG12B810F1024GM64=1' '-D__FPU_PRESENT=1' -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/gecko_sdk_3.1.1/hardware/board support/inc" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/Utilities" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/Modules/Mic" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/Operation Modes/Standard Mode" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/Modules/Audio Analysis" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/gecko_sdk_3.1.1/platform/service/legacy_hal/inc" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/gecko_sdk_3.1.1/platform/service/legacy_hal/src" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/gecko_sdk_3.1.1/hardware/board support/src" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/gecko_sdk_3.1.1/platform/CMSIS/Include" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/gecko_sdk_3.1.1/hardware/board support/config" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/gecko_sdk_3.1.1/hardware/board support/inc" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/gecko_sdk_3.1.1/platform/common/toolchain/inc" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/gecko_sdk_3.1.1/hardware/board/inc" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/gecko_sdk_3.1.1/platform/service/device_init/inc" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/gecko_sdk_3.1.1/platform/common/inc" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/gecko_sdk_3.1.1/platform/service/system/inc" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/gecko_sdk_3.1.1/platform/Device/SiliconLabs/EFM32GG12B/Include" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/gecko_sdk_3.1.1/hardware/board support/config" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/gecko_sdk_3.1.1/hardware/board support/src" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/Modules/Gen_Com" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/Modules/Mic" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/Modules/USB_Com" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/config" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/autogen" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/Modules/Gen_Com" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/Modules/USB_Com" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/gecko_sdk_3.1.1/platform/emlib/inc" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/gecko_sdk_3.1.1/platform/emlib/src" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12" -I"/Users/kevinimlay/Developer/Capstone/BiVo-Backend/BiVo_EMF32GG12/gecko_sdk_3.1.1/platform/CMSIS/src" -O0 -Wall -fno-builtin -fomit-frame-pointer -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -c -fmessage-length=0 -MMD -MP -MF"Modules/Audio Analysis/audio_analysis.d" -MT"Modules/Audio\ Analysis/audio_analysis.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


