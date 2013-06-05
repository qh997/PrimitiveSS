%include  "boot/defs.inc"
%include  "sys/ints.inc"

extern  current
extern  tss
extern  k_reenter

[section .text]
global  restart

save:
    ; 此时 eip/cs/eflags/esp/ss 已经压栈
    ; call save 的返回地址也压栈，esp 指向 proc->regs.eax
    pushad        ; ┓
    push   ds     ; ┃
    push   es     ; ┣ 保护现场
    push   fs     ; ┃
    push   gs     ; ┛

    mov    esi, edx ; 暂存 edx（第四个参数）
    mov    dx, ss
    mov    ds, dx
    mov    es, dx
    mov    fs, dx
    mov    edx, esi

    mov    esi, esp                     ; 此时 esi 为进程表起始地址

    inc    dword [k_reenter]
    cmp    dword [k_reenter], 0         ; 如果 k_reenter != 0，则表示中断重入
    jne    .1                           ; k_reenter != 0
    mov    esp, STK_TOP                 ; 切换到内核栈，在此之前绝对不能进行栈操作
    push   restart                      ; 非中断重入
    jmp    [esi + RETADR - P_STACKBASE] ; 返回
    .1:                                 ;
    push   restart_reenter              ; 中断重入
    jmp    [esi + RETADR - P_STACKBASE] ; 返回
                                        ; 由于存在栈切换并且压栈的值没有弹出
                                        ; 所以不能使用 ret 直接返回。

restart:                                 ; 非中断重入
    mov    esp, [current]                ; 切换到进程表栈
    lldt   [esp + P_LDT_SEL]             ; 加载该进程的 LDT
    lea    eax, [esp + P_STACKTOP]       ; ┓ 下一次中断发生时，esp 将变成 s_proc.regs 的末地址
    mov    dword [tss + TSS3_S_SP0], eax ; ┻ 然后再将该进程的 ss/esp/eflags/cs/eip 压栈
restart_reenter:                         ; 中断重入
    dec    dword [k_reenter]             ;
    pop    gs                            ; ┓
    pop    fs                            ; ┃
    pop    es                            ; ┃
    pop    ds                            ; ┣ 恢复现场
    popad                                ; ┛
    add    esp, 4                        ; 跳过 retaddr
    iretd                                ; pop eip/cs/eflags/esp/ss
