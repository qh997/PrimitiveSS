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

NASM_FLAGS := -I $(INC_PATH)/ -f elf32
GCC_FLAGS := -I $(INC_PATH)/ -c -m32 -std=c99 -fno-builtin -nostdlib -Wall -fomit-frame-pointer

OBJS += $(OUT_DIR)/kernel.o
OBJS += $(OUT_DIR)/init.o
OBJS += $(OUT_DIR)/global.o
OBJS += $(OUT_DIR)/PIC8259A.o
OBJS += $(OUT_DIR)/protect.o
OBJS += $(OUT_DIR)/traps.o
OBJS += $(OUT_DIR)/clock.o
OBJS += $(OUT_DIR)/main.o
OBJS += $(OUT_DIR)/sched.o
OBJS += $(OUT_DIR)/ints.o

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
../out/kernel/init.o: init.c ../include/sys/global.h \
 ../include/sys/protect.h ../include/types.h ../include/sys/proto.h \
 ../include/string.h ../include/types.h ../include/sys/interrupts.h \
 ../include/stdio.h ../include/sys/sched.h ../include/sys/system.h

../out/kernel/global.o: global.c ../include/sys/protect.h \
 ../include/types.h

../out/kernel/PIC8259A.o: PIC8259A.c ../include/sys/proto.h \
 ../include/types.h ../include/sys/interrupts.h

../out/kernel/protect.o: protect.c ../include/sys/protect.h \
 ../include/types.h ../include/sys/interrupts.h ../include/sys/global.h \
 ../include/stdio.h ../include/types.h ../include/string.h

../out/kernel/traps.o: traps.c ../include/sys/interrupts.h \
 ../include/sys/protect.h ../include/types.h ../include/stdio.h \
 ../include/types.h

../out/kernel/clock.o: clock.c ../include/sys/interrupts.h \
 ../include/sys/proto.h ../include/types.h ../include/stdio.h \
 ../include/types.h ../include/sys/sched.h ../include/sys/protect.h \
 ../include/sys/system.h

../out/kernel/main.o: main.c ../include/stdio.h ../include/types.h \
 ../include/sys/sched.h ../include/sys/protect.h ../include/types.h \
 ../include/sys/system.h

../out/kernel/sched.o: sched.c ../include/sys/sched.h \
 ../include/sys/protect.h ../include/types.h ../include/sys/system.h \
 ../include/stdio.h ../include/types.h ../include/string.h

../out/kernel/kernel.o: kernel.s ../include/boot/defs.inc

../out/kernel/ints.o: ints.s ../include/boot/defs.inc \
  ../include/sys/ints.inc

