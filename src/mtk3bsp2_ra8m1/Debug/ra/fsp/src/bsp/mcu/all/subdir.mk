################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/bsp/mcu/all/bsp_clocks.c \
../ra/fsp/src/bsp/mcu/all/bsp_common.c \
../ra/fsp/src/bsp/mcu/all/bsp_delay.c \
../ra/fsp/src/bsp/mcu/all/bsp_group_irq.c \
../ra/fsp/src/bsp/mcu/all/bsp_guard.c \
../ra/fsp/src/bsp/mcu/all/bsp_io.c \
../ra/fsp/src/bsp/mcu/all/bsp_irq.c \
../ra/fsp/src/bsp/mcu/all/bsp_macl.c \
../ra/fsp/src/bsp/mcu/all/bsp_register_protection.c \
../ra/fsp/src/bsp/mcu/all/bsp_rom_registers.c \
../ra/fsp/src/bsp/mcu/all/bsp_sbrk.c \
../ra/fsp/src/bsp/mcu/all/bsp_security.c 

C_DEPS += \
./ra/fsp/src/bsp/mcu/all/bsp_clocks.d \
./ra/fsp/src/bsp/mcu/all/bsp_common.d \
./ra/fsp/src/bsp/mcu/all/bsp_delay.d \
./ra/fsp/src/bsp/mcu/all/bsp_group_irq.d \
./ra/fsp/src/bsp/mcu/all/bsp_guard.d \
./ra/fsp/src/bsp/mcu/all/bsp_io.d \
./ra/fsp/src/bsp/mcu/all/bsp_irq.d \
./ra/fsp/src/bsp/mcu/all/bsp_macl.d \
./ra/fsp/src/bsp/mcu/all/bsp_register_protection.d \
./ra/fsp/src/bsp/mcu/all/bsp_rom_registers.d \
./ra/fsp/src/bsp/mcu/all/bsp_sbrk.d \
./ra/fsp/src/bsp/mcu/all/bsp_security.d 

OBJS += \
./ra/fsp/src/bsp/mcu/all/bsp_clocks.o \
./ra/fsp/src/bsp/mcu/all/bsp_common.o \
./ra/fsp/src/bsp/mcu/all/bsp_delay.o \
./ra/fsp/src/bsp/mcu/all/bsp_group_irq.o \
./ra/fsp/src/bsp/mcu/all/bsp_guard.o \
./ra/fsp/src/bsp/mcu/all/bsp_io.o \
./ra/fsp/src/bsp/mcu/all/bsp_irq.o \
./ra/fsp/src/bsp/mcu/all/bsp_macl.o \
./ra/fsp/src/bsp/mcu/all/bsp_register_protection.o \
./ra/fsp/src/bsp/mcu/all/bsp_rom_registers.o \
./ra/fsp/src/bsp/mcu/all/bsp_sbrk.o \
./ra/fsp/src/bsp/mcu/all/bsp_security.o 

SREC += \
mtk3bsp2_ra8m1.srec 

MAP += \
mtk3bsp2_ra8m1.map 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/bsp/mcu/all/%.o: ../ra/fsp/src/bsp/mcu/all/%.c
	$(file > $@.in,-mthumb -mfloat-abi=hard -march=armv8.1-m.main+mve.fp+fp.dp -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -g -gdwarf-4 -D_RENESAS_RA_ -D_RA_CORE=CM85 -D_RA_ORDINAL=1 -D_RAFSP_EK_RA8M1_ -I"D:/20_workspace/electronic_worke_club/nabe-lid-hikitaro/develop/src/mtk3bsp2_ra8m1/src" -I"." -I"D:/20_workspace/electronic_worke_club/nabe-lid-hikitaro/develop/src/mtk3bsp2_ra8m1/ra/fsp/inc" -I"D:/20_workspace/electronic_worke_club/nabe-lid-hikitaro/develop/src/mtk3bsp2_ra8m1/ra/fsp/inc/api" -I"D:/20_workspace/electronic_worke_club/nabe-lid-hikitaro/develop/src/mtk3bsp2_ra8m1/ra/fsp/inc/instances" -I"D:/20_workspace/electronic_worke_club/nabe-lid-hikitaro/develop/src/mtk3bsp2_ra8m1/ra/arm/CMSIS_5/CMSIS/Core/Include" -I"D:/20_workspace/electronic_worke_club/nabe-lid-hikitaro/develop/src/mtk3bsp2_ra8m1/ra_gen" -I"D:/20_workspace/electronic_worke_club/nabe-lid-hikitaro/develop/src/mtk3bsp2_ra8m1/ra_cfg/fsp_cfg/bsp" -I"D:/20_workspace/electronic_worke_club/nabe-lid-hikitaro/develop/src/mtk3bsp2_ra8m1/ra_cfg/fsp_cfg" -I"D:/20_workspace/electronic_worke_club/nabe-lid-hikitaro/develop/src/mtk3bsp2_ra8m1/mtk3_bsp2" -I"D:/20_workspace/electronic_worke_club/nabe-lid-hikitaro/develop/src/mtk3bsp2_ra8m1/mtk3_bsp2/config" -I"D:/20_workspace/electronic_worke_club/nabe-lid-hikitaro/develop/src/mtk3bsp2_ra8m1/mtk3_bsp2/include" -I"D:/20_workspace/electronic_worke_club/nabe-lid-hikitaro/develop/src/mtk3bsp2_ra8m1/mtk3_bsp2/mtkernel/kernel/knlinc" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -flax-vector-conversions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

