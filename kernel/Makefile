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
GCC_FLAGS := -I $(INC_PATH)/ -c -m32 -std=c99 -fno-builtin -nostdlib -Wall -fomit-frame-pointer -o

OBJS += $(OUT_DIR)/kernel.o
OBJS += $(OUT_DIR)/init.o
OBJS += $(OUT_DIR)/global.o
OBJS += $(OUT_DIR)/PIC8259A.o
OBJS += $(OUT_DIR)/protect.o
OBJS += $(OUT_DIR)/traps.o
OBJS += $(OUT_DIR)/clock.o
OBJS += $(OUT_DIR)/main.o

OUT_OBJS := $(foreach obj, $(OBJS), $(subst $(ROOT_DIR),., $(obj)))

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

$(OUT_DIR)/%.o: %.asm
	@$(SHOW_CPL_ASM) $@ $(TREAT_PATH)
	@$(AS) $(NASM_FLAGS) $@ $<

$(OUT_DIR)/%.o: %.c
	@$(SHOW_CPL_C) $@ $(TREAT_PATH)
	@$(CC) $(GCC_FLAGS) $@ $<
