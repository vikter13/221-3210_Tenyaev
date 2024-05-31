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


include se_masm.inc

EENTER_PROLOG MACRO

ifdef I386
    push    ebp
    mov     ebp, esp
    push    ebx
    push    esi
    push    edi
endif

ifdef X64
    mov         [rsp + 1*SE_WORDSIZE], rcx
    mov         [rsp + 2*SE_WORDSIZE], rdx
    mov         [rsp + 3*SE_WORDSIZE], r8
    mov         [rsp + 4*SE_WORDSIZE], r9

    push_reg    rbp

    push_reg    rbx
    push_reg    rsi
    push_reg    rdi
    push_reg    r12
    push_reg    r13
    push_reg    r14
    push_reg    r15

    alloc_stack(40)
    set_frame   rbp, (12 * SE_WORDSIZE)
    .endprolog
endif

    ENDM


EENTER_EPILOG MACRO

    mov          xbx, [xbp - SE_WORDSIZE * 1]
    mov          xsi, [xbp - SE_WORDSIZE * 2]
    mov          xdi, [xbp - SE_WORDSIZE * 3]
ifdef X64
    mov          r12, [xbp - SE_WORDSIZE * 4]
    mov          r13, [xbp - SE_WORDSIZE * 5]
    mov          r14, [xbp - SE_WORDSIZE * 6]
    mov          r15, [xbp - SE_WORDSIZE * 7]
endif
    mov          xsp, xbp
    pop          xbp

;ifdef X64
    ;add         rsp, 40
    ;pop         r15
    ;pop         r14
    ;pop         r13
    ;pop         r12
;endif
    ;pop         xdi
    ;pop         xsi
    ;pop         xbx
    ;mov         xsp, xbp
    ;pop         xbp
;
    ENDM


frame_arg0  EQU    [xbp + 2*SE_WORDSIZE]
frame_arg1  EQU    [xbp + 3*SE_WORDSIZE]
frame_arg2  EQU    [xbp + 4*SE_WORDSIZE]
frame_arg3  EQU    [xbp + 5*SE_WORDSIZE]
frame_arg4  EQU    [xbp + 6*SE_WORDSIZE]

;refer to sgx_error.h
SE_ERROR_READ_LOCK_FAIL EQU 0c0002202h