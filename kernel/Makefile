MAKE := make
AS   := nasm
CC   := gcc
LD   := ld
AR   := ar

SHOW_CPL_ASM := @echo -n ' COMPILE   ASM    '; echo echo
SHOW_CPL_C   := @echo -n ' COMPILE     C    '; echo echo
SHOW_LNK_OBJ := @echo -n '    LINK  OBJS    '; echo echo
SHOW_INSTALL := @echo -n ' INSTALL          '; echo echo
TREAT_PATH   := | sed 's/.*out\///'
SILENCE      := > /dev/null 2>&1

ROOT_DIR   := ..
OUT_DIR    := $(ROOT_DIR)/out/kernel
BOOT_IMG   := $(ROOT_DIR)/boot.img
KERNEL_MAP := $(OUT_DIR)/KERNEL.map

INC_PATH := ../include
INCLUDES := 

NASM_FLAGS := -I $(INC_PATH)/ -f elf32 -o
GCC_FLAGS := -I $(INC_PATH)/ -m32 -c -std=c99 -fno-builtin -nostdlib -Wall -o
LD_FLAGS := -s -m elf_i386 -Map $(KERNEL_MAP) \
            -Ttext `sed -n '/^KERNEL_OFFSET/s/^.* \([a-fA-F0-9]*\)h/0x\1/p' ../boot/include/defs.inc` \
            -o

KERNEL_OBJS += $(OUT_DIR)/kernel.o
KERNEL_OBJS += $(OUT_DIR)/kernel_init.o

KERNEL_BIN  := $(OUT_DIR)/kernel.bin

.PHONY: everything all new init clean install install_kernel

everything: init $(KERNEL_BIN)

all: everything install

new: clean all

init:
	@mkdir -p $(OUT_DIR)

clean:
	@echo '  clean kernel ...'
	@rm -rf $(OUT_DIR)

install: install_kernel

install_kernel: $(KERNEL_BIN)
	@$(SHOW_INSTALL) $< $(TREAT_PATH)
	dd if=$^ of=$(BOOT_IMG) bs=1 \
	    count=`ls -l $^ | awk -F " " '{print $$5}'` \
	    seek=`echo "obase=10;ibase=16;(\`sed -n '/^LOADER_SECTOR/s/^.* \([a-fA-F0-9]*\)/\1/p' ../boot/include/defs.inc\` + 1) * 200" | bc` \
	    conv=notrunc $(SILENCE)

$(KERNEL_BIN): $(KERNEL_OBJS)
	@$(SHOW_LNK_OBJ) $@ $(TREAT_PATH)
	@$(LD) $(LD_FLAGS) $@ $(KERNEL_OBJS)

$(OUT_DIR)/kernel.o: kernel.asm
	@$(SHOW_CPL_ASM) $< $(TREAT_PATH)
	@$(AS) $(NASM_FLAGS) $@ $<

$(OUT_DIR)/kernel_init.o: kernel_init.c
	@$(SHOW_CPL_C) $< $(TREAT_PATH)
	@$(CC) $(GCC_FLAGS) $@ $<
