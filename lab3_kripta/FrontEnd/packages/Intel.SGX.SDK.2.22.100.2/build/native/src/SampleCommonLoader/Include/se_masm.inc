; Copyright(C) 2011-2018 Intel Corporation All Rights Reserved.
;
; The source code, information  and  material ("Material") contained herein is
; owned  by Intel Corporation or its suppliers or licensors, and title to such
; Material remains  with Intel Corporation  or its suppliers or licensors. The
; Material  contains proprietary information  of  Intel or  its  suppliers and
; licensors. The  Material is protected by worldwide copyright laws and treaty
; provisions. No  part  of  the  Material  may  be  used,  copied, reproduced,
; modified, published, uploaded, posted, transmitted, distributed or disclosed
; in any way  without Intel's  prior  express written  permission. No  license
; under  any patent, copyright  or  other intellectual property rights  in the
; Material  is  granted  to  or  conferred  upon  you,  either  expressly,  by
; implication, inducement,  estoppel or  otherwise.  Any  license  under  such
; intellectual  property  rights must  be express  and  approved  by  Intel in
; writing.
;
;  *Third Party trademarks are the property of their respective owners.
;
; Unless otherwise  agreed  by Intel  in writing, you may not remove  or alter
; this  notice or  any other notice embedded  in Materials by Intel or Intel's
; suppliers or licensors in any way.


ifdef X64
ifndef __TLIBC__
;; c.f. Macros defined in ksamd64.inc
;; http://msdn.microsoft.com/en-us/library/ms235217.aspx
include  ksamd64.inc
endif
endif

ifdef I386
    UNDER   EQU <_>
    _FRAME_ EQU <>
    SE3func EQU __SE3
endif

ifdef X64
    UNDER   EQU <>
    _FRAME_ EQU <FRAME>
    SE3func EQU _SE3
endif

BEGIN_FUNC MACRO   func
    PUBLIC @catstr(%UNDER,<func>)
    @catstr(%UNDER,<func>) PROC _FRAME_
ENDM

BEGIN_FUNC_NAKED    MACRO   func
    PUBLIC @catstr(%UNDER,<func>)
    @catstr(%UNDER,<func>) PROC
ENDM

END_FUNC    MACRO   func
    @catstr(%UNDER,<func>) ENDP
ENDM

ifdef I386
    xax equ     <eax>
    xbx equ     <ebx>
    xcx equ     <ecx>
    xdx equ     <edx>
    xsi equ     <esi>
    xdi equ     <edi>
    xsp equ     <esp>
    xbp equ     <ebp>

    SE_WORDSIZE equ 4
endif

ifdef X64
    xax equ     <rax>
    xbx equ     <rbx>
    xcx equ     <rcx>
    xdx equ     <rdx>
    xsi equ     <rsi>
    xdi equ     <rdi>
    xsp equ     <rsp>
    xbp equ     <rbp>

    SE_WORDSIZE equ 8
endif

    SE_PAGESIZE equ 1000h

;; Generic argument picker for "naked" functions
ifdef I386
    naked_arg0  EQU     4[esp]
    naked_arg1  EQU     8[esp]
    naked_arg2  EQU    12[esp]
    naked_arg3  EQU    16[esp]
endif

;; In Win64, the first four parameters are passed by RCX, RDX, R8, R9.
ifdef X64
    naked_arg0  EQU     rcx
    naked_arg1  EQU     rdx
    naked_arg2  EQU     r8
    naked_arg3  EQU     r9
endif

;; Prolog and epilog for SE instructions, such as EREPORT, EGETKEY, ECREATE...
;; set xbx = arg0, xcx = arg1, xdx = arg2
SE_PROLOG MACRO
ifdef I386
    push        ebp
    mov         ebp, esp

    push        ebx

    mov         ebx, [xbp + 2*SE_WORDSIZE]
    mov         ecx, [xbp + 3*SE_WORDSIZE]
    mov         edx, [xbp + 4*SE_WORDSIZE]
endif

ifdef X64
    push_reg    rbx
    .endprolog

    mov         rbx, rcx
    mov         rcx, rdx
    mov         rdx, r8
endif
    ENDM

SE_EPILOG MACRO
ifdef I386
    pop         ebx
    mov         esp, ebp
    pop         ebp
endif

ifdef X64
    pop         rbx
endif
    ENDM

;; SE leaf codes - need to sync with inst.h
SE_EREPORT  EQU     0
SE_EGETKEY  EQU     1
SE_EENTER   EQU     2
SE_ERESUME  EQU     3
SE_EEXIT    EQU     4

SE_ECREATE  EQU     0
SE_EADD     EQU     1
SE_EINIT    EQU     2
SE_EREMOVE  EQU     3
SE_EDBGRD   EQU     4
SE_EDBGWR   EQU     5
SE_EEXTEND  EQU     6

ifdef SE_SIM
EXTRN   SE3func:PROC
SE_SIM_EXCEPTION EQU 1
endif

ifdef SE_SIM
SGX_ENCLU   MACRO
        local @call_SE3func, @se3_generic, @check_EEXIT
ifdef SE_SIM_EXCEPTION
        cmp  eax, SE_ERESUME
        jne  @se3_generic
        ;; arrage se instruction here is for align to _do_ecall_filter
        ;; so that _do_ecall_filter can detect it is EREUSME exception
        ;; or exception within enclave.
        DB      00Fh
        DB      001h
        DB      0D7h
;       DB      0F3h
@se3_generic:
endif

        cmp eax, SE_EENTER
        jne @check_EEXIT
        mov xdx, xbp
        jmp @call_SE3func
@check_EEXIT:
        cmp  eax, SE_EEXIT
        jne  @call_SE3func

        ; if inst is EEXIT, xbp and xsp need to be passed by xdx and xcx
        mov xdx, xbp
        mov xcx, xsp

@call_SE3func:
ifdef I386
        push edi
        push esi
        push edx
        push ecx
        push ebx
        push eax
endif

ifdef X64
        sub  rsp, 030h
        mov  [rsp + 4*SE_WORDSIZE], rsi
        mov  [rsp + 5*SE_WORDSIZE], rdi
        mov  r9, rdx
        mov  r8, rcx
        mov  rdx, rbx
        mov  rcx, rax
endif

        call SE3func

        add  xsp, SE_WORDSIZE * 6
        ENDM
else
SGX_ENCLU   MACRO
        DB      00Fh
        DB      001h
        DB      0D7h
;		DB      0F3h
        ENDM
endif

;; vim: set syntax=asm ts=4 sw=4 ai et:
