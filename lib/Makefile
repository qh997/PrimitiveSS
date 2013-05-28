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
OUT_DIR    := $(ROOT_DIR)/out/lib
BOOT_IMG   := $(ROOT_DIR)/boot.img
OBJS_OUT   := $(ROOT_DIR)/objs

INC_PATH := ../include
INCLUDES := 

NASM_FLAGS := -I $(INC_PATH)/boot/ -f elf32 -o

OBJS += $(OUT_DIR)/lib_a.o

OUT_OBJS := $(foreach obj, $(OBJS), $(subst ..,., $(obj)))

.PHONY: everything all new init clean

everything: init $(OBJS)

all: everything install

new: clean all

init:
	@mkdir -p $(OUT_DIR)
	@echo -n $(OUT_OBJS) >> $(OBJS_OUT)

clean:
	@echo '  clean lib ...'
	@rm -rf $(OUT_DIR)

$(OUT_DIR)/lib_a.o: lib_a.asm
	@$(SHOW_CPL_ASM) $@ $(TREAT_PATH)
	@$(AS) $(NASM_FLAGS) $@ $<
