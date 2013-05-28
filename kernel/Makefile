MAKE := make
AS   := nasm
CC   := gcc
LD   := ld
AR   := ar

SHOW_CPL_ASM := @echo -n ' COMPILE   ASM    '; echo echo
SHOW_CPL_C   := @echo -n ' COMPILE     C    '; echo echo
TREAT_PATH   := | sed 's/.*out\///'
SILENCE      := > /dev/null 2>&1

ROOT_DIR   := ..
OUT_DIR    := $(ROOT_DIR)/out/kernel
OBJS_OUT   := $(ROOT_DIR)/objs

INC_PATH := ../include
INCLUDES := 

NASM_FLAGS := -I $(INC_PATH)/ -f elf32 -o
GCC_FLAGS := -I $(INC_PATH)/ -m32 -c -std=c99 -fno-builtin -nostdlib -Wall -o

OBJS += $(OUT_DIR)/kernel.o
OBJS += $(OUT_DIR)/init.o
OBJS += $(OUT_DIR)/global.o

OUT_OBJS := $(foreach obj, $(OBJS), $(subst ..,., $(obj)))

.PHONY: everything all new init clean

everything: init $(OBJS)

all: everything

new: clean all

init:
	@mkdir -p $(OUT_DIR)
	@echo -n "$(OUT_OBJS) " >> $(OBJS_OUT)

clean:
	@echo '  clean kernel ...'
	@rm -rf $(OUT_DIR)

install: install_kernel

$(OUT_DIR)/kernel.o: kernel.asm
	@$(SHOW_CPL_ASM) $@ $(TREAT_PATH)
	@$(AS) $(NASM_FLAGS) $@ $<

$(OUT_DIR)/init.o: init.c
	@$(SHOW_CPL_C) $@ $(TREAT_PATH)
	@$(CC) $(GCC_FLAGS) $@ $<

$(OUT_DIR)/global.o: global.c
	@$(SHOW_CPL_C) $@ $(TREAT_PATH)
	@$(CC) $(GCC_FLAGS) $@ $<
