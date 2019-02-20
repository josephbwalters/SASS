################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.oer4f: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7R4 --code_state=16 --float_support=VFPv3D16 -me -O3 --include_path="C:/Users/jwalters/Documents/ti_workspace/mmw_mss_16xx" --include_path="C:/ti/mmwave_sdk_02_00_00_04/packages" --include_path="C:/ti/ti-cgt-arm_16.9.6.LTS/include" --define=SOC_XWR16XX --define=SUBSYS_MSS --define=DOWNLOAD_FROM_CCS --define=DebugP_ASSERT_ENABLED --define=_LITTLE_ENDIAN --define=MMWAVE_L3RAM_SIZE=0xC0000 -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --enum_type=int --abi=eabi --obj_extension=.oer4f --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-642996721:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-642996721-inproc

build-642996721-inproc: ../mss_mmw.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"C:/ti/xdctools_3_50_04_43_core/xs" --xdcpath="C:/ti/bios_6_53_02_00/packages;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.R4F -p ti.platforms.cortexR:IWR16XX:false:200 -r release -c "C:/ti/ti-cgt-arm_16.9.6.LTS" --compileOptions "--enum_type=int " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-642996721 ../mss_mmw.cfg
configPkg/compiler.opt: build-642996721
configPkg/: build-642996721


