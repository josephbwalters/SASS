################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ti-cgt-arm_16.9.6.LTS

GEN_OPTS__FLAG := --cmd_file="configPkg/compiler.opt"
GEN_CMDS__FLAG := -l"configPkg/linker.cmd"

ORDERED_OBJS += \
"./cli.oer4f" \
"./mss_main.oer4f" \
"../mss_mmw_linker.cmd" \
"../r4f_linker.cmd" \
$(GEN_CMDS__FLAG) \
-llibosal_xwr16xx.aer4f \
-llibesm_xwr16xx.aer4f \
-llibgpio_xwr16xx.aer4f \
-llibsoc_xwr16xx.aer4f \
-llibpinmux_xwr16xx.aer4f \
-llibcrc_xwr16xx.aer4f \
-llibdma_xwr16xx.aer4f \
-llibuart_xwr16xx.aer4f \
-llibmailbox_xwr16xx.aer4f \
-llibmmwavelink_xwr16xx.aer4f \
-llibmmwave_xwr16xx.aer4f \
-llibcli_xwr16xx.aer4f \
-lrtsv7R4_T_le_v3D16_eabi.lib \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
xwr16xx_mmw_mss.xer4f \

EXE_OUTPUTS__QUOTED += \
"xwr16xx_mmw_mss.xer4f" \

BIN_OUTPUTS += \
xwr16xx_mmw_mss.hex \

BIN_OUTPUTS__QUOTED += \
"xwr16xx_mmw_mss.hex" \


# All Target
all:
	@$(MAKE) --no-print-directory pre-build
	@$(MAKE) --no-print-directory main-build

# Main-build Target
main-build: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "xwr16xx_mmw_mss.xer4f"

# Tool invocations
xwr16xx_mmw_mss.xer4f: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7R4 --code_state=16 --float_support=VFPv3D16 -me -O3 --define=SOC_XWR16XX --define=SUBSYS_MSS --define=DOWNLOAD_FROM_CCS --define=DebugP_ASSERT_ENABLED --define=_LITTLE_ENDIAN --define=MMWAVE_L3RAM_SIZE=0xC0000 -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --enum_type=int --abi=eabi --obj_extension=.oer4f -z -m"xwr16xx_mmw_mss.map" --heap_size=0x800 --stack_size=0x800 -i"C:/ti/ti-cgt-arm_16.9.6.LTS/lib" -i"C:/ti/mmwave_sdk_02_00_00_04/packages/ti/control/mmwave/lib" -i"C:/ti/mmwave_sdk_02_00_00_04/packages/ti/control/mmwavelink/lib" -i"C:/ti/mmwave_sdk_02_00_00_04/packages/ti/drivers/crc/lib" -i"C:/ti/mmwave_sdk_02_00_00_04/packages/ti/drivers/dma/lib" -i"C:/ti/mmwave_sdk_02_00_00_04/packages/ti/drivers/esm/lib" -i"C:/ti/mmwave_sdk_02_00_00_04/packages/ti/drivers/gpio/lib" -i"C:/ti/mmwave_sdk_02_00_00_04/packages/ti/drivers/mailbox/lib" -i"C:/ti/mmwave_sdk_02_00_00_04/packages/ti/drivers/osal/lib" -i"C:/ti/mmwave_sdk_02_00_00_04/packages/ti/drivers/pinmux/lib" -i"C:/ti/mmwave_sdk_02_00_00_04/packages/ti/drivers/soc/lib" -i"C:/ti/mmwave_sdk_02_00_00_04/packages/ti/drivers/uart/lib" -i"C:/ti/mmwave_sdk_02_00_00_04/packages/ti/utils/cli/lib" -i"C:/ti/ti-cgt-arm_16.9.6.LTS/include" --reread_libs --disable_auto_rts --define=MMWAVE_L3RAM_SIZE=0xC0000 --diag_warning=225 --diag_wrap=off --display_error_number --warn_sections --xml_link_info="xwr16xx_mmw_mss_linkInfo.xml" --rom_model --unused_section_elimination=on -o "xwr16xx_mmw_mss.xer4f" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

xwr16xx_mmw_mss.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"C:/ti/ti-cgt-arm_16.9.6.LTS/bin/armhex"  -o "xwr16xx_mmw_mss.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "cli.oer4f" "mss_main.oer4f" 
	-$(RM) "cli.d" "mss_main.d" 
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-@echo 'Finished clean'
	-@echo ' '

pre-build:
	-rm -f C:/Users/jwalters/Documents/ti_workspace/mmw_mss_16xx/xwr16xx_mmw_mss.bin
	-@echo ' '

post-build:
	-C:/ti/mmwave_sdk_02_00_00_04/packages/scripts/ImageCreator/xwr16xx/out2rprc/out2rprc.exe xwr16xx_mmw_mss.xer4f xwr16xx_mmw_mss.bin
	-C:/ti/mmwave_sdk_02_00_00_04/packages/scripts/ImageCreator/xwr16xx/multicore_image_generator/MulticoreImageGen.exe LE 37 0x00000006 xwr16xx_mmw.bin 0x35510000 xwr16xx_mmw_mss.bin 0xb5510000 C:/ti/mmwave_sdk_02_00_00_04/firmware/radarss/xwr16xx_radarss_rprc.bin 0xd5510000 C:/Users/jwalters/Documents/ti_workspace/mmw_dss_16xx/Debug/xwr16xx_mmw_dss.bin
	-C:/ti/mmwave_sdk_02_00_00_04/packages/scripts/ImageCreator/xwr16xx/crc_multicore_image/crc_multicore_image.exe xwr16xx_mmw.bin xwr16xx_mmw.bin.tmp
	-C:/ti/mmwave_sdk_02_00_00_04/packages/scripts/ImageCreator/append_bin_crc/gen_bincrc32.exe xwr16xx_mmw.bin
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

