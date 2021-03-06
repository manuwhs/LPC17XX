; generated by ARM C/C++ Compiler, 4.1 [Build 791]
; commandline ArmCC [--debug -c --asm -oi2c.o --depend=i2c.d --cpu=Cortex-M3 --apcs=interwork -O0 -IC:\Keil\ARM\CMSIS\Include -IC:\Keil\ARM\INC\NXP\LPC17xx --omf_browse=i2c.crf OV7670\I2C.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||.text||, CODE, READONLY, ALIGN=2

I2C0_IRQHandler PROC
        PUSH     {r4,lr}
        LDR      r0,|L1.796|
        LDR      r0,[r0,#4]
        UXTB     r4,r0
        TST      r4,#7
        BNE      |L1.250|
        ASRS     r0,r4,#3
        CMP      r0,#0xc
        BCS      |L1.250|
        TBB      [pc,r0]
        DCB      0x71,0x06,0x1f,0x32
        DCB      0x41,0x60,0x87,0xba
        DCB      0xc3,0xd5,0xe2,0xfa
        MOV      r0,#0
        LDR      r1,|L1.800|
        STR      r0,[r1,#0]  ; WrIndex
        LDR      r1,|L1.804|
        STR      r0,[r1,#0]  ; RdIndex
        LDR      r1,|L1.800|
        LDR      r0,[r1,#0]  ; WrIndex
        ADD      r1,r0,#1
        LDR      r2,|L1.800|
        STR      r1,[r2,#0]  ; WrIndex
        LDR      r1,|L1.808|
        LDRB     r1,[r1,r0]
        LDR      r2,|L1.796|
        STR      r1,[r2,#8]
        MOV      r0,#1
        LDR      r1,|L1.812|
        STR      r0,[r1,#0]  ; I2CMasterState
        MOV      r0,#0x28
        MOV      r1,r2
        STR      r0,[r1,#0x18]
        B        |L1.614|
        MOV      r0,#0
        LDR      r1,|L1.804|
        STR      r0,[r1,#0]  ; RdIndex
        LDR      r1,|L1.800|
        LDR      r0,[r1,#0]  ; WrIndex
        ADD      r1,r0,#1
        LDR      r2,|L1.800|
        STR      r1,[r2,#0]  ; WrIndex
        LDR      r1,|L1.808|
        LDRB     r1,[r1,r0]
        LDR      r2,|L1.796|
        STR      r1,[r2,#8]
        MOV      r0,#0x28
        MOV      r1,r2
        STR      r0,[r1,#0x18]
        B        |L1.614|
        LDR      r1,|L1.800|
        LDR      r0,[r1,#0]  ; WrIndex
        ADD      r1,r0,#1
        LDR      r2,|L1.800|
        STR      r1,[r2,#0]  ; WrIndex
        LDR      r1,|L1.808|
        LDRB     r1,[r1,r0]
        LDR      r2,|L1.796|
        STR      r1,[r2,#8]
        MOV      r0,#8
        MOV      r1,r2
        STR      r0,[r1,#0x18]
        B        |L1.614|
        LDR      r0,|L1.816|
        LDRB     r0,[r0,#0]  ; I2C_mode
        CMP      r0,#1
        BNE      |L1.184|
        LDR      r1,|L1.800|
        LDR      r0,[r1,#0]  ; WrIndex
        ADD      r1,r0,#1
        LDR      r2,|L1.800|
        STR      r1,[r2,#0]  ; WrIndex
        LDR      r1,|L1.808|
        LDRB     r1,[r1,r0]
        LDR      r2,|L1.796|
        STR      r1,[r2,#8]
        B        |L1.206|
|L1.184|
        MOV      r0,#8
        LDR      r1,|L1.796|
        STR      r0,[r1,#0x18]
        MOV      r0,#0x10
        STR      r0,[r1,#0]
        MOV      r0,#0x103
        LDR      r1,|L1.812|
        STR      r0,[r1,#0]  ; I2CMasterState
|L1.206|
        MOV      r0,#8
        LDR      r1,|L1.796|
        STR      r0,[r1,#0x18]
        B        |L1.614|
        LDR      r0,|L1.800|
        LDR      r0,[r0,#0]  ; WrIndex
        LDR      r1,|L1.820|
        LDR      r1,[r1,#0]  ; I2CWriteLength
        CMP      r0,r1
        BCS      |L1.252|
        LDR      r1,|L1.800|
        LDR      r0,[r1,#0]  ; WrIndex
        ADD      r1,r0,#1
        LDR      r2,|L1.800|
        STR      r1,[r2,#0]  ; WrIndex
        LDR      r1,|L1.808|
        LDRB     r1,[r1,r0]
        LDR      r2,|L1.796|
        STR      r1,[r2,#8]
        B        |L1.284|
|L1.250|
        B        |L1.590|
|L1.252|
        LDR      r0,|L1.824|
        LDR      r0,[r0,#0]  ; I2CReadLength
        CBZ      r0,|L1.268|
        MOV      r0,#0x20
        LDR      r1,|L1.796|
        STR      r0,[r1,#0]
        B        |L1.284|
|L1.268|
        MOV      r0,#0x101
        LDR      r1,|L1.812|
        STR      r0,[r1,#0]  ; I2CMasterState
        MOV      r0,#0x10
        LDR      r1,|L1.796|
        STR      r0,[r1,#0]
|L1.284|
        MOV      r0,#8
        LDR      r1,|L1.796|
        STR      r0,[r1,#0x18]
        B        |L1.614|
        LDR      r0,|L1.816|
        LDRB     r0,[r0,#0]  ; I2C_mode
        CMP      r0,#1
        BNE      |L1.370|
        LDR      r0,|L1.800|
        LDR      r0,[r0,#0]  ; WrIndex
        LDR      r1,|L1.820|
        LDR      r1,[r1,#0]  ; I2CWriteLength
        CMP      r0,r1
        BCS      |L1.336|
        LDR      r1,|L1.800|
        LDR      r0,[r1,#0]  ; WrIndex
        ADD      r1,r0,#1
        LDR      r2,|L1.800|
        STR      r1,[r2,#0]  ; WrIndex
        LDR      r1,|L1.808|
        LDRB     r1,[r1,r0]
        LDR      r2,|L1.796|
        STR      r1,[r2,#8]
        B        |L1.386|
|L1.336|
        LDR      r0,|L1.824|
        LDR      r0,[r0,#0]  ; I2CReadLength
        CBZ      r0,|L1.352|
        MOV      r0,#0x20
        LDR      r1,|L1.796|
        STR      r0,[r1,#0]
        B        |L1.386|
|L1.352|
        MOV      r0,#0x101
        LDR      r1,|L1.812|
        STR      r0,[r1,#0]  ; I2CMasterState
        MOV      r0,#0x10
        LDR      r1,|L1.796|
        STR      r0,[r1,#0]
        B        |L1.386|
|L1.370|
        MOV      r0,#0x10
        LDR      r1,|L1.796|
        STR      r0,[r1,#0]
        MOV      r0,#0x102
        LDR      r1,|L1.812|
        STR      r0,[r1,#0]  ; I2CMasterState
|L1.386|
        MOV      r0,#8
        LDR      r1,|L1.796|
        STR      r0,[r1,#0x18]
        B        |L1.614|
        MOV      r0,#0x104
        LDR      r1,|L1.812|
        STR      r0,[r1,#0]  ; I2CMasterState
        MOV      r0,#8
        LDR      r1,|L1.796|
        STR      r0,[r1,#0x18]
        B        |L1.614|
        LDR      r0,|L1.824|
        LDR      r0,[r0,#0]  ; I2CReadLength
        CMP      r0,#1
        BNE      |L1.432|
        MOV      r0,#4
        LDR      r1,|L1.796|
        STR      r0,[r1,#0x18]
        B        |L1.440|
|L1.432|
        MOV      r0,#4
        LDR      r1,|L1.796|
        STR      r0,[r1,#0]
|L1.440|
        MOV      r0,#8
        LDR      r1,|L1.796|
        STR      r0,[r1,#0x18]
        B        |L1.614|
        MOV      r0,#0x10
        LDR      r1,|L1.796|
        STR      r0,[r1,#0]
        MOV      r0,#0x103
        LDR      r1,|L1.812|
        STR      r0,[r1,#0]  ; I2CMasterState
        MOV      r0,#8
        LDR      r1,|L1.796|
        STR      r0,[r1,#0x18]
        B        |L1.614|
        LDR      r1,|L1.796|
        LDR      r1,[r1,#8]
        UXTB     r2,r1
        LDR      r1,|L1.804|
        LDR      r0,[r1,#0]  ; RdIndex
        ADD      r1,r0,#1
        LDR      r3,|L1.804|
        STR      r1,[r3,#0]  ; RdIndex
        LDR      r1,|L1.828|
        STRB     r2,[r1,r0]
        LDR      r0,|L1.824|
        LDR      r0,[r0,#0]  ; I2CReadLength
        SUB      r0,r0,#1
        MOV      r1,r3
        LDR      r1,[r1,#0]  ; RdIndex
        CMP      r0,r1
        BLS      |L1.526|
        MOV      r0,#4
        LDR      r1,|L1.796|
        STR      r0,[r1,#0]
        B        |L1.534|
        B        |L1.544|
|L1.526|
        MOV      r0,#4
        LDR      r1,|L1.796|
        STR      r0,[r1,#0x18]
|L1.534|
        MOV      r0,#8
        LDR      r1,|L1.796|
        STR      r0,[r1,#0x18]
        B        |L1.614|
|L1.544|
        LDR      r1,|L1.796|
        LDR      r1,[r1,#8]
        UXTB     r2,r1
        LDR      r1,|L1.804|
        LDR      r0,[r1,#0]  ; RdIndex
        ADD      r1,r0,#1
        LDR      r3,|L1.804|
        STR      r1,[r3,#0]  ; RdIndex
        LDR      r1,|L1.828|
        STRB     r2,[r1,r0]
        MOV      r0,#0x101
        LDR      r1,|L1.812|
        STR      r0,[r1,#0]  ; I2CMasterState
        MOV      r0,#0x10
        LDR      r1,|L1.796|
        STR      r0,[r1,#0]
        MOV      r0,#8
        STR      r0,[r1,#0x18]
        B        |L1.614|
|L1.590|
        MOV      r0,#0x10
        LDR      r1,|L1.796|
        STR      r0,[r1,#0]
        ADR      r0,|L1.832|
        BL       tx_cadena_UART0
        MOV      r0,#8
        LDR      r1,|L1.796|
        STR      r0,[r1,#0x18]
        NOP      
|L1.614|
        NOP      
        POP      {r4,pc}
        ENDP

Init_I2C0 PROC
        PUSH     {r4,lr}
        LDR      r0,|L1.860|
        LDR      r0,[r0,#0]
        ORR      r0,r0,#0x80
        LDR      r1,|L1.864|
        STR      r0,[r1,#0xc4]
        MOV      r0,r1
        LDR      r0,[r0,#0x1a8]
        BIC      r0,r0,#0xc000
        STR      r0,[r1,#0x1a8]
        LDR      r0,|L1.868|
        LDR      r0,[r0,#4]
        BIC      r0,r0,#0x3c00000
        LDR      r1,|L1.868|
        STR      r0,[r1,#4]
        MOV      r0,r1
        LDR      r0,[r0,#4]
        ORR      r0,r0,#0x1400000
        STR      r0,[r1,#4]
        LDR      r0,|L1.796|
        LDR      r0,[r0,#0x18]
        ORR      r0,r0,#0x7c
        LDR      r1,|L1.796|
        STR      r0,[r1,#0x18]
        LDR      r0,|L1.868|
        LDR      r0,[r0,#0x7c]
        BIC      r0,r0,#5
        LDR      r1,|L1.868|
        STR      r0,[r1,#0x7c]
        MOV      r0,#0x1f40
        LDR      r1,|L1.796|
        STR      r0,[r1,#0x14]
        STR      r0,[r1,#0x10]
        MOV      r0,#0xa
        MOV      r1,#4
        CMP      r0,#0
        BGE      |L1.738|
        LSL      r2,r1,#27
        LSR      r4,r2,#24
        LDR      r2,|L1.872|
        AND      r3,r0,#0xf
        SUB      r3,r3,#4
        STRB     r4,[r2,r3]
        B        |L1.750|
|L1.738|
        LSL      r2,r1,#27
        LSR      r3,r2,#24
        LDR      r2,|L1.876|
        STRB     r3,[r2,r0]
|L1.750|
        NOP      
        MOV      r0,#0xa
        MOV      r1,#1
        LSL      r1,r1,r0
        LSR      r2,r0,#5
        MOV      r3,#0xe000e000
        ADD      r2,r3,r2,LSL #2
        STR      r1,[r2,#0x100]
        NOP      
        LDR      r0,|L1.796|
        LDR      r0,[r0,#0]
        ORR      r0,r0,#0x40
        LDR      r1,|L1.796|
        STR      r0,[r1,#0]
        POP      {r4,pc}
        ENDP

|L1.796|
        DCD      0x4001c000
|L1.800|
        DCD      WrIndex
|L1.804|
        DCD      RdIndex
|L1.808|
        DCD      I2CMasterBuffer
|L1.812|
        DCD      I2CMasterState
|L1.816|
        DCD      I2C_mode
|L1.820|
        DCD      I2CWriteLength
|L1.824|
        DCD      I2CReadLength
|L1.828|
        DCD      I2CSlaveBuffer
|L1.832|
        DCB      "Situacion desconocida I2C\n\r",0
|L1.860|
        DCD      0x400fc0c4
|L1.864|
        DCD      0x400fc000
|L1.868|
        DCD      0x4002c000
|L1.872|
        DCD      0xe000ed18
|L1.876|
        DCD      0xe000e400

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        AREA ||.bss||, DATA, NOINIT, ALIGN=0

I2CMasterBuffer
        %        35
I2CSlaveBuffer
        %        32
SCCB_WriteQueue
        %        200

        AREA ||.conststring||, DATA, READONLY, MERGE=1, STRINGS, ALIGN=0

        DCB      "Un registro configurado con ",233,"xito\n\r",0

        AREA ||.data||, DATA, ALIGN=2

frase
        DCD      ||.conststring||
I2CMasterState
        DCD      0x00000000
I2CSlaveState
        DCD      0x00000000
RdIndex
        DCD      0x00000000
WrIndex
        DCD      0x00000000
I2C_mode
        DCB      0x01
numWriteQueue
        DCB      0x00,0x00,0x00
I2CReadLength
        DCD      0x00000000
I2CWriteLength
        DCD      0x00000000

;*** Start embedded assembler ***

#line 1 "OV7670\\I2C.c"
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__asm___5_I2C_c_frase____REV16|
#line 114 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cmInstr.h"
|__asm___5_I2C_c_frase____REV16| PROC
#line 115

 rev16 r0, r0
 bx lr
	ENDP
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__asm___5_I2C_c_frase____REVSH|
#line 128
|__asm___5_I2C_c_frase____REVSH| PROC
#line 129

 revsh r0, r0
 bx lr
	ENDP

;*** End   embedded assembler ***

        EXPORT I2C0_IRQHandler [CODE]
        EXPORT Init_I2C0 [CODE]
        EXPORT I2CMasterBuffer [DATA,SIZE=35]
        EXPORT I2CSlaveBuffer [DATA,SIZE=32]
        EXPORT SCCB_WriteQueue [DATA,SIZE=200]
        EXPORT frase [DATA,SIZE=4]
        EXPORT I2CMasterState [DATA,SIZE=4]
        EXPORT I2CSlaveState [DATA,SIZE=4]
        EXPORT RdIndex [DATA,SIZE=4]
        EXPORT WrIndex [DATA,SIZE=4]
        EXPORT I2C_mode [DATA,SIZE=1]
        EXPORT numWriteQueue [DATA,SIZE=1]
        EXPORT I2CReadLength [DATA,SIZE=4]
        EXPORT I2CWriteLength [DATA,SIZE=4]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT tx_cadena_UART0 [CODE]

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
