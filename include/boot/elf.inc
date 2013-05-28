
H_PHOFF  equ  01ch
H_PHNUM  equ  02ch

_dispose_kernel:
    xor    esi, esi                              ;
    mov    cx, word [KERNEL_BIN_ADDR + H_PHNUM]  ; ┓ 44 bytes = 2ch
    movzx  ecx, cx                               ; ┻ ecx = pELFHdr->e_phnum
    mov    esi, [KERNEL_BIN_ADDR + H_PHOFF]      ; 28 bytes = 1ch
    add    esi, KERNEL_BIN_ADDR                  ; esi = KERNEL_BIN_ADDR + pELFHdr->e_phoff
    .begin:                                      ; for (i = e_phnum; i > 0; --i) {
        mov    eax, [esi + 0]                    ;     \\ Elf32_Phdr->p_type
        cmp    eax, 0                            ;     if (p_type == 0)
        jz     .NoAction                         ;         continue;
        push   dword [esi + 010h]                ;     \\ Elf32_Phdr->p_filesz
        mov    eax, [esi + 04h]                  ;     \\ Elf32_Phdr->p_offset
        add    eax, KERNEL_BIN_ADDR              ;
        push   eax                               ;
        push   dword [esi + 08h]                 ;     \\ Elf32_Phdr->p_vaddr
        call   _memcpy                           ;     memcpy((void *)p_vaddr, B21r + p_offset, p_filesz);
        add    esp, 12                           ;
        .NoAction:                               ;
            add    esi, 020h                     ;
            dec    ecx                           ;
            jnz    .begin                        ; }
    ret
