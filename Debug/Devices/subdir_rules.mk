################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Devices/ESP8266.obj: ../Devices/ESP8266.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/ti/simplelink_msp432_sdk_1_40_01_00/source" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/alex/workspace_v7/ESP8266" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Devices/ESP8266.d" --obj_directory="Devices" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Devices/MSPIO.obj: ../Devices/MSPIO.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccsv7/ccs_base/arm/include" --include_path="C:/ti/simplelink_msp432_sdk_1_40_01_00/source" --include_path="C:/ti/ccsv7/ccs_base/arm/include/CMSIS" --include_path="C:/Users/alex/workspace_v7/ESP8266" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Devices/MSPIO.d" --obj_directory="Devices" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


