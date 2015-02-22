################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../gsm.c \
../port.c \
../SiemensC55_ATmega8A_CallServer.c \
../uart.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
gsm.o \
port.o \
SiemensC55_ATmega8A_CallServer.o \
uart.o

OBJS_AS_ARGS +=  \
gsm.o \
port.o \
SiemensC55_ATmega8A_CallServer.o \
uart.o

C_DEPS +=  \
gsm.d \
port.d \
SiemensC55_ATmega8A_CallServer.d \
uart.d

C_DEPS_AS_ARGS +=  \
gsm.d \
port.d \
SiemensC55_ATmega8A_CallServer.d \
uart.d

OUTPUT_FILE_PATH +=SiemensC55_ATmega8A_CallServer.elf

OUTPUT_FILE_PATH_AS_ARGS +=SiemensC55_ATmega8A_CallServer.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler









./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG -DF_CPU=2000000  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega8a -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="SiemensC55_ATmega8A_CallServer.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega8a  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "SiemensC55_ATmega8A_CallServer.elf" "SiemensC55_ATmega8A_CallServer.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "SiemensC55_ATmega8A_CallServer.elf" "SiemensC55_ATmega8A_CallServer.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "SiemensC55_ATmega8A_CallServer.elf" > "SiemensC55_ATmega8A_CallServer.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "SiemensC55_ATmega8A_CallServer.elf" "SiemensC55_ATmega8A_CallServer.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-size.exe" "SiemensC55_ATmega8A_CallServer.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "SiemensC55_ATmega8A_CallServer.elf" "SiemensC55_ATmega8A_CallServer.a" "SiemensC55_ATmega8A_CallServer.hex" "SiemensC55_ATmega8A_CallServer.lss" "SiemensC55_ATmega8A_CallServer.eep" "SiemensC55_ATmega8A_CallServer.map" "SiemensC55_ATmega8A_CallServer.srec" "SiemensC55_ATmega8A_CallServer.usersignatures"
	