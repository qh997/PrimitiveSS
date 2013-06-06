%include  "boot/defs.inc"
%include  "sys/ints.inc"

extern  current
extern  tss
extern  k_reenter
extern  hwirq_table

[section .text]
global  disable_hwirq
global  enable_hwirq
global  restart
global  hwint00
global  hwint01
global  hwint02
global  hwint03
global  hwint04
global  hwint05
global  hwint06
global  hwint07
global  hwint08
global  hwint09
global  hwint10
global  hwint11
global  hwint12
global  hwint13
global  hwint14
global  hwint15

disable_hwirq:
    mov    ecx, [esp + 4]        ;# ecx = irq
    sub    ecx, 20h              ;#
    pushf                        ;#
    cli                          ;# 关中断，将在 iretd 时打开
    mov    ah, 1                 ;#
    rol    ah, cl                ;# ah = 1 << (irq % 8)
    cmp    cl, 8                 ;#
    jae    disable_8             ;# if (cl < 8) // 主 8259A
    disable_0:                   ;# {
        in     al, INT_M_CTLMASK ;#
        test   al, ah            ;#     if (中断已经禁用)
        jnz    dis_already       ;#         return 0
        or     al, ah            ;#
        out    INT_M_CTLMASK, al ;#     屏蔽该中断
        popf                     ;#
        mov    eax, 1            ;#     return 1
        ret                      ;# }
    disable_8:                   ;# else { // 从 8259A
        in     al, INT_S_CTLMASK ;#
        test   al, ah            ;#     if (中断已经禁用)
        jnz    dis_already       ;#         return 0
        or     al, ah            ;#
        out    INT_S_CTLMASK, al ;#     屏蔽该中断
        popf                     ;#
        mov    eax, 1            ;#     return 1
        ret                      ;# }
    dis_already:
        popf
        xor    eax, eax
        ret

enable_hwirq:
    mov    ecx, [esp + 4]
    sub    ecx, 20h
    pushf
    cli
    mov    ah, ~1
    rol    ah, cl
    cmp    cl, 8
    jae    enable_8
    enable_0:
        in     al, INT_M_CTLMASK
        and    al, ah
        out    INT_M_CTLMASK, al
        popf
        ret
    enable_8:
        in     al, INT_S_CTLMASK
        and    al, ah
        out    INT_S_CTLMASK, al
        popf
        ret

save:
    ;# 此时 eip/cs/eflags/esp/ss 已经压栈
    ;# call save 的返回地址也压栈，esp 指向 proc->regs.eax
    pushad          ;# ┓
    push   ds       ;# ┃
    push   es       ;# ┣ 保护现场
    push   fs       ;# ┃
    push   gs       ;# ┛

    mov    esi, edx ;# 暂存 edx（第四个参数）
    mov    dx, ss
    mov    ds, dx
    mov    es, dx
    mov    fs, dx
    mov    edx, esi

    mov    esi, esp                     ;# 此时 esi 为进程表起始地址

    inc    dword [k_reenter]
    cmp    dword [k_reenter], 0         ;# 如果 k_reenter != 0，则表示中断重入
    jne    .1                           ;# k_reenter != 0
    mov    esp, STK_TOP                 ;# 切换到内核栈，在此之前绝对不能进行栈操作
    push   restart                      ;# 非中断重入
    jmp    [esi + RETADR - P_STACKBASE] ;# 返回
    .1:                                 ;#
    push   restart_reenter              ;# 中断重入
    jmp    [esi + RETADR - P_STACKBASE] ;# 返回
                                        ;# 由于存在栈切换并且压栈的值没有弹出
                                        ;# 所以不能使用 ret 直接返回。

restart:                                 ;# 非中断重入
    mov    esp, [current]                ;# 切换到进程表栈
    lldt   [esp + P_LDT_SEL]             ;# 加载该进程的 LDT
    lea    eax, [esp + P_STACKTOP]       ;# ┓ 下一次中断发生时，esp 将变成 s_proc.regs 的末地址
    mov    dword [tss + TSS3_S_SP0], eax ;# ┻ 然后再将该进程的 ss/esp/eflags/cs/eip 压栈
restart_reenter:                         ;# 中断重入
    dec    dword [k_reenter]             ;#
    pop    gs                            ;# ┓
    pop    fs                            ;# ┃
    pop    es                            ;# ┃
    pop    ds                            ;# ┣ 恢复现场
    popad                                ;# ┛
    add    esp, 4                        ;# 跳过 retaddr
    iretd                                ;# pop eip/cs/eflags/esp/ss

;## 硬件中断 ##
    ALIGN  16
    hwint00:            ;# <clock>
        hwint_master  0
    ALIGN  16
    hwint01:            ;# <keyboard>
        hwint_master  1
    ALIGN  16
    hwint02:            ;# <cascade!>
        hwint_master  2
    ALIGN  16
    hwint03:            ;# <second serial>
        hwint_master  3
    ALIGN  16
    hwint04:            ;# <first serial>
        hwint_master  4
    ALIGN  16
    hwint05:            ;# <XT winchester>
        hwint_master  5
    ALIGN  16
    hwint06:            ;# <floppy>
        hwint_master  6
    ALIGN  16
    hwint07:            ;# <printer>
        hwint_master  7
    ALIGN  16
    hwint08:            ;# <realtime clock>
        hwint_slave   8
    ALIGN  16
    hwint09:            ;# <irq 2 redirected>
        hwint_slave   9
    ALIGN  16
    hwint10:            ;#
        hwint_slave  10
    ALIGN  16
    hwint11:            ;#
        hwint_slave  11
    ALIGN  16
    hwint12:            ;#
        hwint_slave  12
    ALIGN  16
    hwint13:            ;# <FPU exception>
        hwint_slave  13
    ALIGN  16
    hwint14:            ;# <AT winchester>
        hwint_slave  14
    ALIGN  16
    hwint15:            ;#
        hwint_slave  15
