#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include "types.h"

#define INT_SYSCALL 0x80

typedef int (*syscall_handler)();

#define _syscall0(type, name, vect)      \
    type name(void)                      \
    {                                    \
        long __res;                      \
        __asm__ volatile (               \
            "int $0x80"                  \
            : "=a" (__res)               \
            : "0" (vect),                \
              "b" ((long)(0)),           \
              "c" ((long)(0)),           \
              "d" ((long)(0))            \
        );                               \
        return (type) __res;             \
    }

#define _syscall3(type, name, vect,      \
                  typea, typeb, typec)   \
    type name(typea a, typeb b, typec c) \
    {                                    \
        long __res;                      \
        __asm__ volatile (               \
            "int $0x80"                  \
            : "=a" (__res)               \
            : "0" (vect),                \
              "b" ((long)(a)),           \
              "c" ((long)(b)),           \
              "d" ((long)(c))            \
        );                               \
        return (type) __res;             \
    }

#endif
