%include  "sys/ints.inc"

extern  current
extern  tss
extern  k_reenter

[section .text]
align  32
global  restart

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
