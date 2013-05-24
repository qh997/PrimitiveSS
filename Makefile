MAKE := make
AS   := nasm
CC   := gcc
LD   := ld
AR   := ar

MAKE_SUB := $(MAKE) -s -C

TRIP_PATH := | sed 's/.*out\///'
SHOW_CPL_AS := @echo -n ' COMPILE   ASM    '; echo echo
SHOW_CPL_CC := @echo -n ' COMPILE    CC    '; echo echo
SHOW_LNK_LD := @echo -n ' LINK       LD    '; echo echo
SHOW_PKG_AR := @echo -n ' COMPILE    AR    '; echo echo
SHOW_INSTAL := @echo -n ' INSTALL          '; echo echo

ROOT_DIR := .
OUT_DIR := $(ROOT_DIR)/out
BOOT_DIR := $(ROOT_DIR)/boot

.PHONY: everything all new init clean install boot

everything: init \
	        boot
	@echo

all: everything install

new: clean all

init:
	@mkdir -p $(OUT_DIR)

clean:
	@$(MAKE_SUB) $(BOOT_DIR) $@
	@echo '  clean out ...'
	@rm -rf $(OUT_DIR)
	@echo

install:
	@$(MAKE_SUB) $(BOOT_DIR) $@
	@echo

boot:
	@$(MAKE_SUB) $(BOOT_DIR)
