MAKE := make
AS   := nasm
CC   := gcc
LD   := ld
AR   := ar

SHOW_CPL_ASM := @echo -n ' COMPILE   ASM    '; echo echo
SHOW_INSTALL := @echo -n ' INSTALL          '; echo echo
TREAT_PATH  := | sed 's/.*\///'
SILENCE      := > /dev/null 2>&1

ROOT_DIR := ..
OUT_DIR  := $(ROOT_DIR)/out
BOOT_IMG := $(ROOT_DIR)/boot.img

INC_PATH := ../include

NASM_FLAGS := -I $(INC_PATH)/

BOOT_BIN   := $(OUT_DIR)/boot.bin
LOADER_BIN := $(OUT_DIR)/loader.bin

TARGETS += $(BOOT_BIN)
TARGETS += $(LOADER_BIN)

.PHONY: everything all new init clean install install_boot

everything: init $(TARGETS)

all: everything install

new: clean all

init:

clean:
	@echo '  clean boot ...'
	@rm -f $(BOOT_BIN)
	@rm -f $(LOADER_BIN)

dep:
	@sed '/^### Dependencies ###$$/q' < Makefile > tmp_Makefile
	@(for tfile in $(TARGETS); do \
	      sfile=`echo $${tfile##*$(OUT_DIR)/} | sed "s/\.bin/.s/"`; \
	      [ -f $${sfile} ] || continue; \
	      $(AS) $(NASM_FLAGS) -M -MT $${tfile} $${sfile}; \
	  done) >> tmp_Makefile
	@mv tmp_Makefile Makefile

$(OUT_DIR)/%.bin: %.s
	@$(SHOW_CPL_ASM) $@ $(TREAT_PATH)
	@$(AS) $(NASM_FLAGS) -o $@ $<

install: install_boot install_loader

install_boot: $(BOOT_BIN)
	$(SHOW_INSTALL) $< $(TREAT_PATH)
	@dd if=$^ of=$(BOOT_IMG) bs=512 count=1 conv=notrunc $(SILENCE)

install_loader: $(LOADER_BIN)
	$(SHOW_INSTALL) $< $(TREAT_PATH)
	@dd if=$^ of=$(BOOT_IMG) seek=512 bs=1 \
        count=`ls -l $(LOADER_BIN) | awk -F " " '{print $$5}'` \
        conv=notrunc $(SILENCE)

### Dependencies ###
../out/boot.bin: boot.s ../include/boot/defs.inc \
  ../include/boot/io_rm.inc

../out/loader.bin: loader.s ../include/boot/protect.inc \
  ../include/boot/defs.inc ../include/boot/io_rm.inc \
  ../include/boot/io_pm.inc ../include/boot/elf.inc

