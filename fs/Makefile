AS   := nasm
CC   := gcc
LD   := ld
AR   := ar

SHOW_CPL_ASM := @echo -n ' COMPILE   ASM    '; echo echo
SHOW_CPL_C   := @echo -n ' COMPILE     C    '; echo echo
TREAT_PATH   := | sed 's/.*out\///'
SILENCE      := > /dev/null 2>&1

ROOT_DIR   := ..
OUT_DIR    := $(ROOT_DIR)/out/fs
OBJS_OUT   := $(ROOT_DIR)/objs

INC_PATH := ../include

NASM_FLAGS := -I $(INC_PATH)/ -f elf32
GCC_FLAGS := -I $(INC_PATH)/ -c -m32 -std=c99 -fno-builtin -nostdlib -Wall -fomit-frame-pointer

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

dep:
	@sed '/^### Dependencies ###$$/q' < Makefile > tmp_Makefile
	@(for tfile in $(OBJS); do \
	      sfile=`echo $${tfile##*$(OUT_DIR)/} | sed "s/\.o/.c/"`; \
	      [ -f $${sfile} ] || continue; \
	      $(CC) $(GCC_FLAGS) -MM -MT $${tfile} $${sfile}; \
	      echo; \
	  done) >> tmp_Makefile
	@(for tfile in $(OBJS); do \
	      sfile=`echo $${tfile##*$(OUT_DIR)/} | sed "s/\.o/.s/"`; \
	      [ -f $${sfile} ] || continue; \
	      $(AS) $(NASM_FLAGS) -M -MT $${tfile} $${sfile}; \
	  done) >> tmp_Makefile
	@mv tmp_Makefile Makefile

$(OUT_DIR)/%.o: %.s
	@$(SHOW_CPL_ASM) $@ $(TREAT_PATH)
	@$(AS) $(NASM_FLAGS) -o $@ $<

$(OUT_DIR)/%.o: %.c
	@$(SHOW_CPL_C) $@ $(TREAT_PATH)
	@$(CC) $(GCC_FLAGS) -o $@ $<

### Dependencies ###
../out/fs/main.o: main.c

