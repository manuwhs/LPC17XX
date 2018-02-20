; generated by ARM C/C++ Compiler, 4.1 [Build 791]
; commandline ArmCC [--debug -c --asm -olpc17xx_clkpwr.o --depend=lpc17xx_clkpwr.d --cpu=Cortex-M3 --apcs=interwork -O0 -IC:\Keil\ARM\CMSIS\Include -IC:\Keil\ARM\INC\NXP\LPC17xx --omf_browse=lpc17xx_clkpwr.crf lpc17xx_clkpwr.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||.text||, CODE, READONLY, ALIGN=2

CLKPWR_SetPCLKDiv PROC
        PUSH     {r4,lr}
        MOV      r2,r1
        CMP      r0,#0x20
        BCS      |L1.12|
        MOV      r3,r0
        B        |L1.16|
|L1.12|
        SUB      r3,r0,#0x20
|L1.16|
        MOV      r1,r3
        CMP      r0,#0x20
        BCS      |L1.64|
        LDR      r3,|L1.328|
        LDR      r3,[r3,#0]
        MOV      r4,#3
        LSL      r4,r4,r1
        BIC      r3,r3,r4
        LDR      r4,|L1.332|
        STR      r3,[r4,#0x1a8]
        LDR      r3,|L1.328|
        LDR      r3,[r3,#0]
        LSL      r4,r2,r1
        ORR      r3,r3,r4
        LDR      r4,|L1.332|
        STR      r3,[r4,#0x1a8]
        B        |L1.104|
|L1.64|
        LDR      r3,|L1.336|
        LDR      r3,[r3,#0]
        MOV      r4,#3
        LSL      r4,r4,r1
        BIC      r3,r3,r4
        LDR      r4,|L1.332|
        STR      r3,[r4,#0x1ac]
        LDR      r3,|L1.336|
        LDR      r3,[r3,#0]
        LSL      r4,r2,r1
        ORR      r3,r3,r4
        LDR      r4,|L1.332|
        STR      r3,[r4,#0x1ac]
|L1.104|
        POP      {r4,pc}
        ENDP

CLKPWR_GetPCLKSEL PROC
        MOV      r1,r0
        CMP      r1,#0x20
        BCS      |L1.120|
        MOV      r2,r1
        LDR      r3,|L1.328|
        LDR      r0,[r3,#0]
        B        |L1.128|
|L1.120|
        SUB      r2,r1,#0x20
        LDR      r3,|L1.336|
        LDR      r0,[r3,#0]
|L1.128|
        LSR      r3,r0,r2
        AND      r0,r3,#3
        BX       lr
        ENDP

CLKPWR_GetPCLK PROC
        PUSH     {r4-r6,lr}
        MOV      r6,r0
        LDR      r0,|L1.340|
        LDR      r4,[r0,#0]  ; SystemCoreClock
        MOV      r0,r6
        BL       CLKPWR_GetPCLKSEL
        MOV      r5,r0
        CBZ      r5,|L1.170|
        CMP      r5,#1
        BEQ      |L1.176|
        CMP      r5,#2
        BEQ      |L1.182|
        CMP      r5,#3
        BNE      |L1.194|
        B        |L1.188|
|L1.170|
        MOV      r5,#4
        B        |L1.194|
|L1.176|
        MOV      r5,#1
        B        |L1.194|
|L1.182|
        MOV      r5,#2
        B        |L1.194|
|L1.188|
        MOV      r5,#8
        NOP      
|L1.194|
        NOP      
        UDIV     r4,r4,r5
        MOV      r0,r4
        POP      {r4-r6,pc}
        ENDP

CLKPWR_ConfigPPWR PROC
        CMP      r1,#1
        BNE      |L1.230|
        LDR      r2,|L1.344|
        LDR      r2,[r2,#0]
        LDR      r3,|L1.348|
        AND      r3,r0,r3
        ORR      r2,r2,r3
        LDR      r3,|L1.332|
        STR      r2,[r3,#0xc4]
        B        |L1.252|
|L1.230|
        CBNZ     r1,|L1.252|
        LDR      r2,|L1.344|
        LDR      r2,[r2,#0]
        LDR      r3,|L1.348|
        BIC      r3,r3,r0
        AND      r2,r2,r3
        LDR      r3,|L1.332|
        STR      r2,[r3,#0xc4]
|L1.252|
        BX       lr
        ENDP

CLKPWR_Sleep PROC
        MOV      r0,#0
        LDR      r1,|L1.352|
        STR      r0,[r1,#0]
        WFI      
        BX       lr
        ENDP

CLKPWR_DeepSleep PROC
        MOV      r0,#4
        LDR      r1,|L1.356|
        STR      r0,[r1,#0]
        MOV      r0,#8
        LDR      r1,|L1.352|
        STR      r0,[r1,#0]
        WFI      
        BX       lr
        ENDP

CLKPWR_PowerDown PROC
        MOV      r0,#4
        LDR      r1,|L1.356|
        STR      r0,[r1,#0]
        MOV      r0,#9
        LDR      r1,|L1.352|
        STR      r0,[r1,#0]
        WFI      
        BX       lr
        ENDP

CLKPWR_DeepPowerDown PROC
        MOV      r0,#4
        LDR      r1,|L1.356|
        STR      r0,[r1,#0]
        MOV      r0,#3
        LDR      r1,|L1.352|
        STR      r0,[r1,#0]
        WFI      
        BX       lr
        ENDP

        DCW      0x0000
|L1.328|
        DCD      0x400fc1a8
|L1.332|
        DCD      0x400fc000
|L1.336|
        DCD      0x400fc1ac
|L1.340|
        DCD      SystemCoreClock
|L1.344|
        DCD      0x400fc0c4
|L1.348|
        DCD      0xefeff7de
|L1.352|
        DCD      0x400fc0c0
|L1.356|
        DCD      0xe000ed10

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

;*** Start embedded assembler ***

#line 1 "lpc17xx_clkpwr.c"
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__asm___16_lpc17xx_clkpwr_c_5f9c9c89____REV16|
#line 114 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cmInstr.h"
|__asm___16_lpc17xx_clkpwr_c_5f9c9c89____REV16| PROC
#line 115

 rev16 r0, r0
 bx lr
	ENDP
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__asm___16_lpc17xx_clkpwr_c_5f9c9c89____REVSH|
#line 128
|__asm___16_lpc17xx_clkpwr_c_5f9c9c89____REVSH| PROC
#line 129

 revsh r0, r0
 bx lr
	ENDP

;*** End   embedded assembler ***

        EXPORT CLKPWR_SetPCLKDiv [CODE]
        EXPORT CLKPWR_GetPCLKSEL [CODE]
        EXPORT CLKPWR_GetPCLK [CODE]
        EXPORT CLKPWR_ConfigPPWR [CODE]
        EXPORT CLKPWR_Sleep [CODE]
        EXPORT CLKPWR_DeepSleep [CODE]
        EXPORT CLKPWR_PowerDown [CODE]
        EXPORT CLKPWR_DeepPowerDown [CODE]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT SystemCoreClock [DATA]

        ATTR FILESCOPE
        ATTR SETVALUE Tag_ABI_PCS_wchar_t,2
        ATTR SETVALUE Tag_ABI_enum_size,1
        ATTR SETVALUE Tag_ABI_optimization_goals,6
        ATTR SETSTRING Tag_conformance,"2.06"
        ATTR SETVALUE AV,18,1

        ASSERT {ENDIAN} = "little"
        ASSERT {INTER} = {TRUE}
        ASSERT {ROPI} = {FALSE}
        ASSERT {RWPI} = {FALSE}
        ASSERT {IEEE_FULL} = {FALSE}
        ASSERT {IEEE_PART} = {FALSE}
        ASSERT {IEEE_JAVA} = {FALSE}
        END
