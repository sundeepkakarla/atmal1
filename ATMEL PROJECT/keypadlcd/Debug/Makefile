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

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../Debug/ \
../Debug/gfx_library/ \
../gfx_library/


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../gfx_library/gfx_mono_c12832_a1z.c \
../gfx_library/gfx_mono_framebuffer.c \
../gfx_library/gfx_mono_generic.c \
../gfx_library/gfx_mono_text.c \
../gfx_library/st7565r.c \
../gfx_library/sysfont.c \
../keypadlcd.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
gfx_library/gfx_mono_c12832_a1z.o \
gfx_library/gfx_mono_framebuffer.o \
gfx_library/gfx_mono_generic.o \
gfx_library/gfx_mono_text.o \
gfx_library/st7565r.o \
gfx_library/sysfont.o \
keypadlcd.o

OBJS_AS_ARGS +=  \
gfx_library/gfx_mono_c12832_a1z.o \
gfx_library/gfx_mono_framebuffer.o \
gfx_library/gfx_mono_generic.o \
gfx_library/gfx_mono_text.o \
gfx_library/st7565r.o \
gfx_library/sysfont.o \
keypadlcd.o

C_DEPS +=  \
gfx_library/gfx_mono_c12832_a1z.d \
gfx_library/gfx_mono_framebuffer.d \
gfx_library/gfx_mono_generic.d \
gfx_library/gfx_mono_text.d \
gfx_library/st7565r.d \
gfx_library/sysfont.d \
keypadlcd.d

C_DEPS_AS_ARGS +=  \
gfx_library/gfx_mono_c12832_a1z.d \
gfx_library/gfx_mono_framebuffer.d \
gfx_library/gfx_mono_generic.d \
gfx_library/gfx_mono_text.d \
gfx_library/st7565r.d \
gfx_library/sysfont.d \
keypadlcd.d

OUTPUT_FILE_PATH +=keypadlcd.elf

OUTPUT_FILE_PATH_AS_ARGS +=keypadlcd.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler















gfx_library/%.o: ../gfx_library/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG -DF_CPU=32000000UL -DGFX_MONO_C12832_A1Z=1  -I"../gfx_library"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=atxmega256a3bu -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG -DF_CPU=32000000UL -DGFX_MONO_C12832_A1Z=1  -I"../gfx_library"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=atxmega256a3bu -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="keypadlcd.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mrelax -mmcu=atxmega256a3bu  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "keypadlcd.elf" "keypadlcd.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "keypadlcd.elf" "keypadlcd.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "keypadlcd.elf" > "keypadlcd.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "keypadlcd.elf" "keypadlcd.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-size.exe" "keypadlcd.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "keypadlcd.elf" "keypadlcd.a" "keypadlcd.hex" "keypadlcd.lss" "keypadlcd.eep" "keypadlcd.map" "keypadlcd.srec" "keypadlcd.usersignatures"
	