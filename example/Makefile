#-----------------------------------------------------------------------------
# 
# sdpff Makefile
# 
# Get started with AVR compiling and flashing
# This simple project gives you a frame easily
# adaptable to use another chip and/or programmer
#
# this GNU-makefile relies on the GCC toolchain
#	
# Hugo Schaaf - ( see https://github.com/hugoschaaf98/ )
# 2019-03-10
#
#-----------------------------------------------------------------------------

#---- Control global settings --------------------------------
#
# ${BIN_PREFIX}* programs will be built
BIN_PREFIX=main
#
# make opt=1 --> enable optimisation, then disable debug
# make opt=0 --> disable optimisation, then enable debug
opt=0
# make debug=1 --> enable debug prints
# make debug=0 --> disable debug prints
debug=0
#
# target chip
MCU=atmega328p
# Clock frequency
F_CPU=16000000
# erase the chip
# make erase=0 --> don't erase the chip
# make erase=1 --> erase the chip
erase=0
# programmer used for flash
programmer=arduino
# programming port used if needed
# make flash port=/dev/<port name>
port=
# baudrate if needed
# note that if you try to flash arduino nano with old bootloader
# the baudrate must be 57600bps instead of 115200bps
baudrate=57600
# fuses - no fuses here, the target chip is an arduino nano bord with the original bootloader
# so we're not able to change the fuses value
# efuses=
# hfuses=
# lfuses=

CC:=avr-gcc
LD:=${CC}
DD:=avrdude
OBJCOPY:=avr-objcopy

#---- directories ------------------------------
#
# headers directory
DINC=./
# source directory
DSRC=src/
# object directory
DOBJ=obj/
# executable directory
DBIN=bin/

VPATH+=${DSRC}:${DOBJ}

#---- detect operating system ------------------------------
#
ifneq (${OS},Windows_NT)
  ifneq (,${findstring Microsoft,${shell cat /proc/version 2>/dev/null}})
	# Windows-Subsystem-for-Linux
	OS:=WSL
  else ifneq (,${findstring Ubuntu,${shell lsb_release -i 2>/dev/null}})
	OS:=Ubuntu
  else
	OS:=${strip ${shell uname -s}}
  endif
endif

ifeq (${OS},Windows_NT)
  SKIP_LINE=echo.
  REMOVE=del /q
else
  SKIP_LINE=echo
  REMOVE=rm -rf
endif

#---- deduce file names ------------------------------------------
#
# find main source file
MAIN_C_FILE=${wildcard ${strip ${DSRC}${BIN_PREFIX}.c}}

# find common source files
COMMON_C_FILES=${filter-out ${MAIN_C_FILE},${wildcard ${DSRC}*.c}}

# deduce object file names from source file names
MAIN_OBJECT_FILE=${sort ${patsubst ${DSRC}%.c,${DOBJ}%.o,${MAIN_C_FILE}}}
COMMON_OBJECT_FILES=${sort ${patsubst ${DSRC}%.c,${DOBJ}%.o,${COMMON_C_FILES}}}
OBJECT_FILES=${MAIN_OBJECT_FILE} ${COMMON_OBJECT_FILES}

# deduce depend file names
DEPEND_FILES=${patsubst %.o,%.d,${OBJECT_FILES}}

# deduce executable file name
BIN_SUFFIX =.elf
TARGET_SUFFIX =.hex
BIN_FILE = ${DBIN}${BIN_PREFIX}${BIN_SUFFIX}
TARGET_FILE = ${DBIN}${BIN_PREFIX}${TARGET_SUFFIX}

# deduce generated file names
GENERATED_FILES=${DEPEND_FILES} ${OBJECT_FILES} ${BIN_FILE} ${TARGET_FILE}

#---- compiler/linker settings ------------------------------
#
CPPFLAGS=
CFLAGS=
LDFLAGS=

CPPFLAGS+=-g -pedantic -Wall -Wextra -Wconversion -MMD -mmcu=${MCU} -DF_CPU=${F_CPU} -fuse-linker-plugin
CFLAGS+=-std=c99 -Wc++-compat -Wwrite-strings -Wold-style-definition

ifneq (${strip ${DINC}},)
	CPPFLAGS+=-I${DINC}
endif

#---- debug/optimisation settings ------------------------------
ifeq (${strip ${opt}},1)
	CPPFLAGS+=-Os
endif

ifeq (${strip ${debug}},1)
	CPPFLAGS+=-DDEBUG
endif

#---- upload settings ----------------------------------------

DDFLAGS = -v -D -p${MCU} -c${programmer} -U flash:w:${TARGET_FILE}:i

#---- programmer adjustements
ifeq (${strip ${programmer}}, arduino)
	DDFLAGS+= -P${port} -b${baudrate}
else 
	# nothing for now
endif

ifeq (${strip ${erase}},1)
	DDFLAGS+= -e
endif


#---- main target ------------------------------
#
all : ${BIN_FILE}
rebuild : clean all

.SUFFIXES:
.SECONDARY:
.PHONY: all flash clean rebuild showf

# linker command to produce the elf files and objcopy command to generate hex file ----
${BIN_FILE} : ${MAIN_OBJECT_FILE} ${COMMON_OBJECT_FILES}
	@echo ==== linking $@ ====
	${LD} -o $@ $^ ${CPPFLAGS} ${CFLAGS} ${LDFLAGS}
	@${SKIP_LINE}

	@echo ==== generating ${TARGET_FILE} ====
	${OBJCOPY} -j .text -j .data -O ihex $@ ${TARGET_FILE}
	@${SKIP_LINE}

# compiler command for every source file ----
${DOBJ}%.o : ${DSRC}%.c
	@echo ==== compiling [opt=${opt}] [debug=${debug}] $< ====
	${CC} -o $@ $< -c ${CPPFLAGS} ${CFLAGS}
	${REMOVE} ${DEPEND_FILES}
	@${SKIP_LINE}

-include ${DEPEND_FILES}

flash :
	@echo ==== flashing [erase=${erase}] ${TARGET_FILE} ====
	${DD} ${DDFLAGS}

# remove generated files ----
clean :
	@echo ==== cleaning ====
	${REMOVE} ${GENERATED_FILES}
	@${SKIP_LINE}

showf :
	@echo source file names : ${MAIN_C_FILE} ${COMMON_C_FILES}
	@echo object file names : ${MAIN_OBJECT_FILE} ${COMMON_OBJECT_FILES}
	@echo depend file names : ${DEPEND_FILES}
	@echo generated files : ${GENERATED_FILES}
	@${SKIP_LINE}

#-----------------------------------------------------------------------------
