#include "sys/global.h"
#include "sys/proto.h"
#include "string.h"
#include "sys/interrupts.h"
#include "stdio.h"
#include "sys/protect.h"
#include "sys/sched.h"

void init()
{
    protect_init();
    trap_init();
    PIC8259A_init();
}
