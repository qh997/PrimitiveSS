
                        ; GD        P  S type
                        ; 0000 ---- 0000 0000
DA_32        equ  4000h ; 0100
DA_4K        equ  8000h ; 1000
DA_DPL0      equ    00h ;           0000
DA_DPL1      equ    20h ;           0010
DA_DPL2      equ    40h ;           0100
DA_DPL3      equ    60h ;           0110
DA_DR        equ    90h ;           1001 0000
DA_DRW       equ    92h ;           1001 0010
DA_DRWA      equ    93h ;           1001 0011
DA_C         equ    98h ;           1001 1000
DA_CR        equ    9Ah ;           1001 1010
DA_CCO       equ    9Ch ;           1001 1100
DA_CCOR      equ    9Eh ;           1001 1110
DA_LDT       equ    82h ;           1000 0010
DA_TaskGate  equ    85h ;           1000 0101
DA_386TSS    equ    89h ;           1000 1001
DA_386CGate  equ    8Ch ;           1000 1100
DA_386IGate  equ    8Eh ;           1000 1110
DA_386TGate  equ    8Fh ;           1000 1111

                 ; -...- TI RPL
                 ; 15..3  2 10
SA_RPL0   equ  0 ;          00
SA_RPL1   equ  1 ;          01
SA_RPL2   equ  2 ;          10
SA_RPL3   equ  3 ;          11

PG_P      equ  1 ; 页存在属性位
PG_RWR    equ  0 ; R/W 属性位值, 读/执行
PG_RWW    equ  2 ; R/W 属性位值, 读/写/执行
PG_USS    equ  0 ; U/S 属性位值, 系统级
PG_USU    equ  4 ; U/S 属性位值, 用户级

PGN       equ  PG_P | PG_USU | PG_RWW

%macro DESCRIPTOR 3
    dw  %2 & 0FFFFh
    dw  %1 & 0FFFFh
    db  (%1 >> 16) & 0FFh
    dw  ((%2 >> 8) & 0F00h) | (%3 & 0F0FFh)
    db  (%1 >> 24) & 0FFh
%endmacro
