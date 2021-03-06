; generated by ARM C/C++ Compiler, 4.1 [Build 791]
; commandline ArmCC [--debug -c --asm -olpc17xx_ssp.o --depend=lpc17xx_ssp.d --cpu=Cortex-M3 --apcs=interwork -O0 -IC:\Keil\ARM\CMSIS\Include -IC:\Keil\ARM\INC\NXP\LPC17xx --omf_browse=lpc17xx_ssp.crf lpc17xx_ssp.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||.text||, CODE, READONLY, ALIGN=2

setSSPclock PROC
        PUSH     {r4-r10,lr}
        MOV      r4,r0
        MOV      r8,r1
        LDR      r0,|L1.1004|
        CMP      r4,r0
        BEQ      |L1.20|
        LDR      r0,|L1.1008|
        CMP      r4,r0
        BNE      |L1.22|
|L1.20|
        B        |L1.30|
|L1.22|
        MOVS     r1,#0x43
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.30|
        LDR      r0,|L1.1004|
        CMP      r4,r0
        BNE      |L1.46|
        MOVS     r0,#0x2a
        BL       CLKPWR_GetPCLK
        MOV      r7,r0
        B        |L1.66|
|L1.46|
        LDR      r0,|L1.1008|
        CMP      r4,r0
        BNE      |L1.62|
        MOVS     r0,#0x14
        BL       CLKPWR_GetPCLK
        MOV      r7,r0
        B        |L1.66|
|L1.62|
        POP      {r4-r10,pc}
|L1.66|
        MOV      r5,#0
        MOV      r9,#0xffffffff
        MOV      r6,#2
        B        |L1.112|
|L1.80|
        ADD      r0,r5,#1
        MUL      r0,r0,r6
        UDIV     r9,r7,r0
        CMP      r9,r8
        BLS      |L1.112|
        ADD      r5,r5,#1
        CMP      r5,#0xff
        BLS      |L1.112|
        MOV      r5,#0
        ADD      r6,r6,#2
|L1.112|
        CMP      r9,r8
        BHI      |L1.80|
        LDR      r0,[r4,#0]
        UXTB     r0,r0
        STR      r0,[r4,#0]
        LDR      r0,[r4,#0]
        MOV      r1,#0xffff
        AND      r1,r1,r5,LSL #8
        ORR      r0,r0,r1
        STR      r0,[r4,#0]
        UXTB     r0,r6
        STR      r0,[r4,#0x10]
        NOP      
        B        |L1.62|
        ENDP

SSP_Init PROC
        PUSH     {r4-r6,lr}
        MOV      r4,r0
        MOV      r5,r1
        LDR      r0,|L1.1004|
        CMP      r4,r0
        BEQ      |L1.164|
        LDR      r0,|L1.1008|
        CMP      r4,r0
        BNE      |L1.166|
|L1.164|
        B        |L1.174|
|L1.166|
        MOVS     r1,#0x81
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.174|
        LDR      r0,|L1.1004|
        CMP      r4,r0
        BNE      |L1.190|
        MOVS     r1,#1
        LSLS     r0,r1,#21
        BL       CLKPWR_ConfigPPWR
        B        |L1.208|
|L1.190|
        LDR      r0,|L1.1008|
        CMP      r4,r0
        BNE      |L1.206|
        MOVS     r1,#1
        ASRS     r0,r0,#20
        BL       CLKPWR_ConfigPPWR
        B        |L1.208|
|L1.206|
        POP      {r4-r6,pc}
|L1.208|
        LDRD     r0,r1,[r5,#4]
        ORR      r0,r0,r1
        LDR      r1,[r5,#0x10]
        ORR      r0,r0,r1
        LDR      r1,[r5,#0]
        ORR      r0,r0,r1
        UXTH     r6,r0
        STR      r6,[r4,#0]
        LDR      r0,[r5,#0xc]
        AND      r6,r0,#0xf
        STR      r6,[r4,#4]
        MOV      r0,r4
        LDR      r1,[r5,#0x14]
        BL       setSSPclock
        NOP      
        B        |L1.206|
        ENDP

SSP_DeInit PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        LDR      r0,|L1.1004|
        CMP      r4,r0
        BEQ      |L1.268|
        LDR      r0,|L1.1008|
        CMP      r4,r0
        BNE      |L1.270|
|L1.268|
        B        |L1.278|
|L1.270|
        MOVS     r1,#0xa8
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.278|
        LDR      r0,|L1.1004|
        CMP      r4,r0
        BNE      |L1.296|
        MOVS     r1,#0
        MOV      r0,#0x200000
        BL       CLKPWR_ConfigPPWR
        B        |L1.310|
|L1.296|
        LDR      r0,|L1.1008|
        CMP      r4,r0
        BNE      |L1.310|
        MOVS     r1,#0
        ASRS     r0,r0,#20
        BL       CLKPWR_ConfigPPWR
|L1.310|
        POP      {r4,pc}
        ENDP

SSP_GetDataSize PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        LDR      r0,|L1.1004|
        CMP      r4,r0
        BEQ      |L1.328|
        LDR      r0,|L1.1008|
        CMP      r4,r0
        BNE      |L1.330|
|L1.328|
        B        |L1.338|
|L1.330|
        MOVS     r1,#0xc0
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.338|
        LDR      r0,[r4,#0]
        AND      r0,r0,#0xf
        POP      {r4,pc}
        ENDP

SSP_ConfigStructInit PROC
        MOV      r1,#0
        STR      r1,[r0,#4]
        STR      r1,[r0,#8]
        LDR      r1,|L1.1028|
        STR      r1,[r0,#0x14]
        MOV      r1,#7
        STR      r1,[r0,#0]
        MOV      r1,#0
        STR      r1,[r0,#0xc]
        STR      r1,[r0,#0x10]
        BX       lr
        ENDP

SSP_Cmd PROC
        PUSH     {r4-r6,lr}
        MOV      r4,r0
        MOV      r5,r1
        LDR      r0,|L1.1004|
        CMP      r4,r0
        BEQ      |L1.392|
        LDR      r0,|L1.1008|
        CMP      r4,r0
        BNE      |L1.394|
|L1.392|
        B        |L1.402|
|L1.394|
        MOVS     r1,#0xe5
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.402|
        CBZ      r5,|L1.408|
        CMP      r5,#1
        BNE      |L1.410|
|L1.408|
        B        |L1.418|
|L1.410|
        MOVS     r1,#0xe6
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.418|
        CMP      r5,#1
        BNE      |L1.432|
        LDR      r0,[r4,#4]
        ORR      r0,r0,#2
        STR      r0,[r4,#4]
        B        |L1.440|
|L1.432|
        LDR      r0,[r4,#4]
        AND      r0,r0,#0xd
        STR      r0,[r4,#4]
|L1.440|
        POP      {r4-r6,pc}
        ENDP

SSP_LoopBackCmd PROC
        PUSH     {r4-r6,lr}
        MOV      r4,r0
        MOV      r5,r1
        LDR      r0,|L1.1004|
        CMP      r4,r0
        BEQ      |L1.460|
        LDR      r0,|L1.1008|
        CMP      r4,r0
        BNE      |L1.462|
|L1.460|
        B        |L1.470|
|L1.462|
        MOVS     r1,#0xfe
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.470|
        CBZ      r5,|L1.476|
        CMP      r5,#1
        BNE      |L1.478|
|L1.476|
        B        |L1.486|
|L1.478|
        MOVS     r1,#0xff
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.486|
        CMP      r5,#1
        BNE      |L1.500|
        LDR      r0,[r4,#4]
        ORR      r0,r0,#1
        STR      r0,[r4,#4]
        B        |L1.508|
|L1.500|
        LDR      r0,[r4,#4]
        AND      r0,r0,#0xe
        STR      r0,[r4,#4]
|L1.508|
        POP      {r4-r6,pc}
        ENDP

SSP_SlaveOutputCmd PROC
        PUSH     {r4-r6,lr}
        MOV      r4,r0
        MOV      r5,r1
        LDR      r0,|L1.1004|
        CMP      r4,r0
        BEQ      |L1.528|
        LDR      r0,|L1.1008|
        CMP      r4,r0
        BNE      |L1.530|
|L1.528|
        B        |L1.540|
|L1.530|
        MOV      r1,#0x11a
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.540|
        CBZ      r5,|L1.546|
        CMP      r5,#1
        BNE      |L1.548|
|L1.546|
        B        |L1.558|
|L1.548|
        MOV      r1,#0x11b
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.558|
        CMP      r5,#1
        BNE      |L1.572|
        LDR      r0,[r4,#4]
        AND      r0,r0,#7
        STR      r0,[r4,#4]
        B        |L1.580|
|L1.572|
        LDR      r0,[r4,#4]
        ORR      r0,r0,#8
        STR      r0,[r4,#4]
|L1.580|
        POP      {r4-r6,pc}
        ENDP

SSP_SendData PROC
        PUSH     {r4-r6,lr}
        MOV      r4,r0
        MOV      r5,r1
        LDR      r0,|L1.1004|
        CMP      r4,r0
        BEQ      |L1.600|
        LDR      r0,|L1.1008|
        CMP      r4,r0
        BNE      |L1.602|
|L1.600|
        B        |L1.612|
|L1.602|
        MOV      r1,#0x134
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.612|
        STR      r5,[r4,#8]
        POP      {r4-r6,pc}
        ENDP

SSP_ReceiveData PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        LDR      r0,|L1.1004|
        CMP      r4,r0
        BEQ      |L1.632|
        LDR      r0,|L1.1008|
        CMP      r4,r0
        BNE      |L1.634|
|L1.632|
        B        |L1.644|
|L1.634|
        MOV      r1,#0x144
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.644|
        LDR      r0,[r4,#8]
        UXTH     r0,r0
        POP      {r4,pc}
        ENDP

SSP_ReadWrite PROC
        PUSH     {r0-r2,r4-r11,lr}
        SUB      sp,sp,#8
        MOV      r6,r0
        MOV      r4,r1
        MOV      r0,#0
        STR      r0,[r4,#0xc]
        STR      r0,[r4,#4]
        STR      r0,[r4,#0x14]
        B        |L1.680|
|L1.672|
        MOV      r0,r6
        BL       SSP_ReceiveData
        MOV      r7,r0
|L1.680|
        LDR      r0,[r6,#0xc]
        TST      r0,#4
        BNE      |L1.672|
        MOV      r0,#3
        STR      r0,[r6,#0x20]
        MOV      r0,r6
        BL       SSP_GetDataSize
        CMP      r0,#8
        BLE      |L1.710|
        MOV      r5,#1
        B        |L1.714|
|L1.710|
        MOV      r5,#0
|L1.714|
        LDR      r0,[sp,#0x10]
        CMP      r0,#0
        BNE      |L1.972|
        CBNZ     r5,|L1.732|
        LDR      r9,[r4,#8]
        LDR      r10,[r4,#0]
        B        |L1.740|
|L1.732|
        LDR      r11,[r4,#8]
        LDR      r0,[r4,#0]
        STR      r0,[sp,#4]
|L1.740|
        B        |L1.950|
|L1.742|
        LDR      r0,[r6,#0xc]
        TST      r0,#2
        BEQ      |L1.854|
        LDR      r1,[r4,#0x10]
        LDR      r0,[r4,#4]
        CMP      r0,r1
        BEQ      |L1.854|
        LDR      r0,[r4,#0]
        CBNZ     r0,|L1.802|
        CBNZ     r5,|L1.782|
        MOVS     r1,#0xff
        MOV      r0,r6
        BL       SSP_SendData
        LDR      r0,[r4,#4]
        ADD      r0,r0,#1
        STR      r0,[r4,#4]
        B        |L1.854|
|L1.782|
        MOV      r1,#0xffff
        MOV      r0,r6
        BL       SSP_SendData
        LDR      r0,[r4,#4]
        ADD      r0,r0,#2
        STR      r0,[r4,#4]
        B        |L1.854|
|L1.802|
        CBNZ     r5,|L1.828|
        LDRB     r1,[r10,#0]
        MOV      r0,r6
        BL       SSP_SendData
        ADD      r10,r10,#1
        LDR      r0,[r4,#4]
        ADD      r0,r0,#1
        STR      r0,[r4,#4]
        B        |L1.854|
|L1.828|
        LDR      r0,[sp,#4]
        LDRH     r1,[r0,#0]
        MOV      r0,r6
        BL       SSP_SendData
        LDR      r0,[sp,#4]
        ADD      r0,r0,#2
        STR      r0,[sp,#4]
        LDR      r0,[r4,#4]
        ADD      r0,r0,#2
        STR      r0,[r4,#4]
|L1.854|
        LDR      r8,[r6,#0x18]
        TST      r8,#1
        BEQ      |L1.880|
        ORR      r0,r8,#0x200
        STR      r0,[r4,#0x14]
        MOV      r0,#0xffffffff
|L1.874|
        ADD      sp,sp,#0x14
        POP      {r4-r11,pc}
|L1.880|
        B        |L1.934|
|L1.882|
        MOV      r0,r6
        BL       SSP_ReceiveData
        MOV      r7,r0
        LDR      r0,[r4,#8]
        CBZ      r0,|L1.914|
        CBNZ     r5,|L1.906|
        STRB     r7,[r9,#0]
        ADD      r9,r9,#1
        B        |L1.914|
|L1.906|
        STRH     r7,[r11,#0]
        ADD      r11,r11,#2
|L1.914|
        CBNZ     r5,|L1.926|
        LDR      r0,[r4,#0xc]
        ADD      r0,r0,#1
        STR      r0,[r4,#0xc]
        B        |L1.934|
|L1.926|
        LDR      r0,[r4,#0xc]
        ADD      r0,r0,#2
        STR      r0,[r4,#0xc]
|L1.934|
        LDR      r0,[r6,#0xc]
        TST      r0,#4
        BEQ      |L1.950|
        LDRD     r0,r1,[r4,#0xc]
        CMP      r0,r1
        BNE      |L1.882|
|L1.950|
        LDR      r1,[r4,#0x10]
        LDR      r0,[r4,#4]
        CMP      r0,r1
        BNE      |L1.742|
        LDRD     r0,r1,[r4,#0xc]
        CMP      r0,r1
        BNE      |L1.742|
        MOV      r0,#0x100
        B        |L1.974|
|L1.972|
        B        |L1.996|
|L1.974|
        STR      r0,[r4,#0x14]
        LDR      r0,[r4,#0]
        CBZ      r0,|L1.984|
        LDR      r0,[r4,#4]
        B        |L1.874|
|L1.984|
        LDR      r0,[r4,#8]
        CBZ      r0,|L1.992|
        LDR      r0,[r4,#0xc]
        B        |L1.874|
|L1.992|
        MOVS     r0,#0
        B        |L1.874|
|L1.996|
        LDR      r0,[sp,#0x10]
        CMP      r0,#1
        BNE      |L1.1248|
        B        |L1.1208|
|L1.1004|
        DCD      0x40088000
|L1.1008|
        DCD      0x40030000
|L1.1012|
        DCB      "lpc17xx_ssp.c",0
        DCB      0
        DCB      0
|L1.1028|
        DCD      0x000f4240
|L1.1032|
        LDR      r0,[r4,#0]
        CBNZ     r0,|L1.1076|
        CBNZ     r5,|L1.1056|
        MOVS     r1,#0xff
        MOV      r0,r6
        BL       SSP_SendData
        LDR      r0,[r4,#4]
        ADD      r0,r0,#1
        STR      r0,[r4,#4]
        B        |L1.1120|
|L1.1056|
        MOV      r1,#0xffff
        MOV      r0,r6
        BL       SSP_SendData
        LDR      r0,[r4,#4]
        ADD      r0,r0,#2
        STR      r0,[r4,#4]
        B        |L1.1120|
|L1.1076|
        CBNZ     r5,|L1.1100|
        LDRD     r0,r2,[r4,#0]
        LDRB     r1,[r0,r2]
        MOV      r0,r6
        BL       SSP_SendData
        LDR      r0,[r4,#4]
        ADD      r0,r0,#1
        STR      r0,[r4,#4]
        B        |L1.1120|
|L1.1100|
        LDRD     r0,r2,[r4,#0]
        LDRH     r1,[r0,r2]
        MOV      r0,r6
        BL       SSP_SendData
        LDR      r0,[r4,#4]
        ADD      r0,r0,#2
        STR      r0,[r4,#4]
|L1.1120|
        LDR      r8,[r6,#0x18]
        TST      r8,#1
        BEQ      |L1.1142|
        ORR      r0,r8,#0x200
        STR      r0,[r4,#0x14]
        MOV      r0,#0xffffffff
        B        |L1.874|
|L1.1142|
        B        |L1.1192|
|L1.1144|
        MOV      r0,r6
        BL       SSP_ReceiveData
        MOV      r7,r0
        LDR      r0,[r4,#8]
        CBZ      r0,|L1.1172|
        CBNZ     r5,|L1.1166|
        LDRD     r1,r2,[r4,#8]
        STRB     r7,[r1,r2]
        B        |L1.1172|
|L1.1166|
        LDRD     r1,r2,[r4,#8]
        STRH     r7,[r1,r2]
|L1.1172|
        CBNZ     r5,|L1.1184|
        LDR      r0,[r4,#0xc]
        ADD      r0,r0,#1
        STR      r0,[r4,#0xc]
        B        |L1.1192|
|L1.1184|
        LDR      r0,[r4,#0xc]
        ADD      r0,r0,#2
        STR      r0,[r4,#0xc]
|L1.1192|
        LDR      r0,[r6,#0xc]
        TST      r0,#4
        BEQ      |L1.1208|
        LDRD     r0,r1,[r4,#0xc]
        CMP      r0,r1
        BNE      |L1.1144|
|L1.1208|
        LDR      r0,[r6,#0xc]
        TST      r0,#2
        BEQ      |L1.1224|
        LDR      r1,[r4,#0x10]
        LDR      r0,[r4,#4]
        CMP      r0,r1
        BNE      |L1.1032|
|L1.1224|
        LDRD     r0,r1,[r4,#0xc]
        CMP      r0,r1
        BNE      |L1.1240|
        LDR      r1,[r4,#0x10]
        LDR      r0,[r4,#4]
        CMP      r0,r1
        BEQ      |L1.1250|
|L1.1240|
        MOV      r0,#0xf
        STR      r0,[r6,#0x14]
        B        |L1.1256|
|L1.1248|
        B        |L1.1260|
|L1.1250|
        MOV      r0,#0x100
        STR      r0,[r4,#0x14]
|L1.1256|
        MOVS     r0,#0
        B        |L1.874|
|L1.1260|
        MOV      r0,#0xffffffff
        B        |L1.874|
        ENDP

SSP_GetStatus PROC
        PUSH     {r4-r6,lr}
        MOV      r5,r0
        MOV      r4,r1
        LDR      r0,|L1.1764|
        CMP      r5,r0
        BEQ      |L1.1284|
        LDR      r0,|L1.1768|
        CMP      r5,r0
        BNE      |L1.1286|
|L1.1284|
        B        |L1.1298|
|L1.1286|
        MOV      r1,#0x214
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.1298|
        CMP      r4,#1
        BEQ      |L1.1318|
        CMP      r4,#2
        BEQ      |L1.1318|
        CMP      r4,#4
        BEQ      |L1.1318|
        CMP      r4,#8
        BEQ      |L1.1318|
        CMP      r4,#0x10
        BNE      |L1.1320|
|L1.1318|
        B        |L1.1332|
|L1.1320|
        MOV      r1,#0x215
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.1332|
        LDR      r0,[r5,#0xc]
        TST      r0,r4
        BEQ      |L1.1342|
        MOVS     r0,#1
|L1.1340|
        POP      {r4-r6,pc}
|L1.1342|
        MOVS     r0,#0
        B        |L1.1340|
        ENDP

SSP_IntConfig PROC
        PUSH     {r4-r6,lr}
        MOV      r4,r0
        MOV      r5,r1
        MOV      r6,r2
        LDR      r0,|L1.1764|
        CMP      r4,r0
        BEQ      |L1.1366|
        LDR      r0,|L1.1768|
        CMP      r4,r0
        BNE      |L1.1368|
|L1.1366|
        B        |L1.1380|
|L1.1368|
        MOV      r1,#0x22c
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.1380|
        CMP      r6,#1
        BNE      |L1.1394|
        LDR      r0,[r4,#0x14]
        ORR      r0,r0,r5
        STR      r0,[r4,#0x14]
        B        |L1.1410|
|L1.1394|
        LDR      r0,[r4,#0x14]
        MOV      r1,#0xf
        BIC      r1,r1,r5
        AND      r0,r0,r1
        STR      r0,[r4,#0x14]
|L1.1410|
        POP      {r4-r6,pc}
        ENDP

SSP_GetRawIntStatus PROC
        PUSH     {r4-r6,lr}
        MOV      r5,r0
        MOV      r4,r1
        LDR      r0,|L1.1764|
        CMP      r5,r0
        BEQ      |L1.1430|
        LDR      r0,|L1.1768|
        CMP      r5,r0
        BNE      |L1.1432|
|L1.1430|
        B        |L1.1444|
|L1.1432|
        MOV      r1,#0x249
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.1444|
        CMP      r4,#1
        BEQ      |L1.1460|
        CMP      r4,#2
        BEQ      |L1.1460|
        CMP      r4,#4
        BEQ      |L1.1460|
        CMP      r4,#8
        BNE      |L1.1462|
|L1.1460|
        B        |L1.1474|
|L1.1462|
        MOV      r1,#0x24a
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.1474|
        LDR      r0,[r5,#0x18]
        TST      r0,r4
        BEQ      |L1.1484|
        MOVS     r0,#1
|L1.1482|
        POP      {r4-r6,pc}
|L1.1484|
        MOVS     r0,#0
        B        |L1.1482|
        ENDP

SSP_GetRawIntStatusReg PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        LDR      r0,|L1.1764|
        CMP      r4,r0
        BEQ      |L1.1504|
        LDR      r0,|L1.1768|
        CMP      r4,r0
        BNE      |L1.1506|
|L1.1504|
        B        |L1.1518|
|L1.1506|
        MOV      r1,#0x258
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.1518|
        LDR      r0,[r4,#0x18]
        POP      {r4,pc}
        ENDP

SSP_GetIntStatus PROC
        PUSH     {r4-r6,lr}
        MOV      r5,r0
        MOV      r4,r1
        LDR      r0,|L1.1764|
        CMP      r5,r0
        BEQ      |L1.1540|
        LDR      r0,|L1.1768|
        CMP      r5,r0
        BNE      |L1.1542|
|L1.1540|
        B        |L1.1554|
|L1.1542|
        MOV      r1,#0x26d
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.1554|
        CMP      r4,#1
        BEQ      |L1.1570|
        CMP      r4,#2
        BEQ      |L1.1570|
        CMP      r4,#4
        BEQ      |L1.1570|
        CMP      r4,#8
        BNE      |L1.1572|
|L1.1570|
        B        |L1.1584|
|L1.1572|
        MOV      r1,#0x26e
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.1584|
        LDR      r0,[r5,#0x1c]
        TST      r0,r4
        BEQ      |L1.1594|
        MOVS     r0,#1
|L1.1592|
        POP      {r4-r6,pc}
|L1.1594|
        MOVS     r0,#0
        B        |L1.1592|
        ENDP

SSP_ClearIntPending PROC
        PUSH     {r4-r6,lr}
        MOV      r4,r0
        MOV      r5,r1
        LDR      r0,|L1.1764|
        CMP      r4,r0
        BEQ      |L1.1616|
        LDR      r0,|L1.1768|
        CMP      r4,r0
        BNE      |L1.1618|
|L1.1616|
        B        |L1.1630|
|L1.1618|
        MOV      r1,#0x281
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.1630|
        CMP      r5,#1
        BEQ      |L1.1638|
        CMP      r5,#2
        BNE      |L1.1640|
|L1.1638|
        B        |L1.1652|
|L1.1640|
        MOV      r1,#0x282
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.1652|
        STR      r5,[r4,#0x20]
        POP      {r4-r6,pc}
        ENDP

SSP_DMACmd PROC
        PUSH     {r4-r6,lr}
        MOV      r4,r0
        MOV      r5,r1
        MOV      r6,r2
        LDR      r0,|L1.1764|
        CMP      r4,r0
        BEQ      |L1.1676|
        LDR      r0,|L1.1768|
        CMP      r4,r0
        BNE      |L1.1678|
|L1.1676|
        B        |L1.1690|
|L1.1678|
        MOV      r1,#0x297
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.1690|
        CMP      r5,#2
        BEQ      |L1.1698|
        CMP      r5,#1
        BNE      |L1.1700|
|L1.1698|
        B        |L1.1712|
|L1.1700|
        MOV      r1,#0x298
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.1712|
        CBZ      r6,|L1.1718|
        CMP      r6,#1
        BNE      |L1.1720|
|L1.1718|
        B        |L1.1732|
|L1.1720|
        MOV      r1,#0x299
        ADR      r0,|L1.1012|
        BL       check_failed
|L1.1732|
        CMP      r6,#1
        BNE      |L1.1746|
        LDR      r0,[r4,#0x24]
        ORR      r0,r0,r5
        STR      r0,[r4,#0x24]
        B        |L1.1762|
|L1.1746|
        LDR      r0,[r4,#0x24]
        MOV      r1,#3
        BIC      r1,r1,r5
        AND      r0,r0,r1
        STR      r0,[r4,#0x24]
|L1.1762|
        POP      {r4-r6,pc}
        ENDP

|L1.1764|
        DCD      0x40088000
|L1.1768|
        DCD      0x40030000

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

;*** Start embedded assembler ***

#line 1 "lpc17xx_ssp.c"
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__asm___13_lpc17xx_ssp_c_SSP_Init____REV16|
#line 114 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cmInstr.h"
|__asm___13_lpc17xx_ssp_c_SSP_Init____REV16| PROC
#line 115

 rev16 r0, r0
 bx lr
	ENDP
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__asm___13_lpc17xx_ssp_c_SSP_Init____REVSH|
#line 128
|__asm___13_lpc17xx_ssp_c_SSP_Init____REVSH| PROC
#line 129

 revsh r0, r0
 bx lr
	ENDP

;*** End   embedded assembler ***

        EXPORT SSP_Init [CODE]
        EXPORT SSP_DeInit [CODE]
        EXPORT SSP_GetDataSize [CODE]
        EXPORT SSP_ConfigStructInit [CODE]
        EXPORT SSP_Cmd [CODE]
        EXPORT SSP_LoopBackCmd [CODE]
        EXPORT SSP_SlaveOutputCmd [CODE]
        EXPORT SSP_SendData [CODE]
        EXPORT SSP_ReceiveData [CODE]
        EXPORT SSP_ReadWrite [CODE]
        EXPORT SSP_GetStatus [CODE]
        EXPORT SSP_IntConfig [CODE]
        EXPORT SSP_GetRawIntStatus [CODE]
        EXPORT SSP_GetRawIntStatusReg [CODE]
        EXPORT SSP_GetIntStatus [CODE]
        EXPORT SSP_ClearIntPending [CODE]
        EXPORT SSP_DMACmd [CODE]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT check_failed [CODE]
        IMPORT CLKPWR_GetPCLK [CODE]
        IMPORT CLKPWR_ConfigPPWR [CODE]

        KEEP setSSPclock

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
