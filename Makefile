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

MAKE_SUB := $(MAKE) -s -C

ROOT_DIR   := .
OBJS_OUT   := $(ROOT_DIR)/objs
OUT_DIR    := $(ROOT_DIR)/out
BOOT_DIR   := $(ROOT_DIR)/boot
KERNEL_DIR := $(ROOT_DIR)/kernel
LIB_DIR    := $(ROOT_DIR)/lib

INC_PATH := ./include

BOOT_IMG   := $(ROOT_DIR)/boot.img
KERNEL_BIN := $(OUT_DIR)/kernel.bin
KERNEL_MAP := $(OUT_DIR)/KERNEL.map

LD_FLAGS := -s -m elf_i386 -Map $(KERNEL_MAP) \
            -Ttext `sed -n '/^KERNEL_OFFSET/s/^.* \([a-fA-F0-9]*\)h/0x\1/p' $(INC_PATH)/boot/defs.inc` \
            -o

OBJECTS += boot
OBJECTS += kernel

.PHONY: everything all new init clean install $(OBJECTS)

everything: init $(OBJECTS) $(KERNEL_BIN)
	@echo

all: everything install

new: clean all

init:
	@mkdir -p $(OUT_DIR)
	@echo -n > $(OBJS_OUT)

clean:
	@$(MAKE_SUB) $(BOOT_DIR) $@
	@$(MAKE_SUB) $(KERNEL_DIR) $@
	@echo '  clean out ...'
	@rm -rf $(OUT_DIR)
	@rm -f $(OBJS_OUT)
	@echo

install: $(KERNEL_BIN)
	@$(MAKE_SUB) $(BOOT_DIR) $@
	@$(SHOW_INSTALL) $< $(TREAT_PATH)
	@dd if=$^ of=$(BOOT_IMG) bs=1 \
        count=`ls -l $^ | awk -F " " '{print $$5}'` \
        seek=`echo "obase=10;ibase=16; \
                 (\`sed -n '/^LOADER_SECTOR/s/^.* \([a-fA-F0-9]*\)/\1/p' \
                 $(INC_PATH)/boot/defs.inc\` + 1) * 200" | bc` \
        conv=notrunc $(SILENCE)
	@echo

boot:
	@$(MAKE_SUB) $(BOOT_DIR)

kernel:
	@$(MAKE_SUB) $(KERNEL_DIR)
	@$(MAKE_SUB) $(LIB_DIR)

$(KERNEL_BIN):
	@$(SHOW_LNK_OBJ) $@ $(TREAT_PATH)
	@$(LD) $(LD_FLAGS) $@ `cat $(OBJS_OUT)`
