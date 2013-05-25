MAKE := make
AS   := nasm
CC   := gcc
LD   := ld
AR   := ar

SHOW_CPL_ASM := @echo -n ' COMPILE   ASM    '; echo echo
SHOW_INSTALL := @echo -n ' INSTALL          '; echo echo
TREAT_PATH   := | sed 's/.*out\///'
SILENCE      := > /dev/null 2>&1

ROOT_DIR := ..
OUT_DIR  := $(ROOT_DIR)/out/boot
BOOT_IMG := $(ROOT_DIR)/boot.img

INC_PATH := ./include
INCLUDES := $(INC_PATH)/defs.inc \
            $(INC_PATH)/io_rm.inc \
            $(INC_PATH)/io_pm.inc

BOOT_BIN   := $(OUT_DIR)/boot.bin
LOADER_BIN := $(OUT_DIR)/loader.bin

.PHONY: everything all new init clean install install_boot

everything: init \
	        $(BOOT_BIN) \
	        $(LOADER_BIN)

all: everything install

new: clean all

init:
	@mkdir -p $(OUT_DIR)

clean:
	@echo '  clean boot ...'
	@rm -rf $(OUT_DIR)

install: install_boot install_loader

install_boot: $(BOOT_BIN)
	$(SHOW_INSTALL) $< $(TREAT_PATH)
	@dd if=$^ of=$(BOOT_IMG) bs=512 count=1 conv=notrunc $(SILENCE)

$(BOOT_BIN): boot.asm $(INCLUDES)
	$(SHOW_CPL_ASM) $@ $(TREAT_PATH)
	@$(AS) -I $(INC_PATH)/ -o $@ $<

install_loader: $(LOADER_BIN)
	$(SHOW_INSTALL) $< $(TREAT_PATH)
	@dd if=$^ of=$(BOOT_IMG) seek=512 bs=1 count=`ls -l $(LOADER_BIN) | awk -F " " '{print $$5}'` \
	    conv=notrunc $(SILENCE)

$(LOADER_BIN): loader.asm $(INCLUDES)
	$(SHOW_CPL_ASM) $@ $(TREAT_PATH)
	@$(AS) -I $(INC_PATH)/ -o $@ $<
