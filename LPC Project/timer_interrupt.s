; generated by ARM C/C++ Compiler, 4.1 [Build 791]
; commandline ArmCC [--debug -c --asm -otimer_interrupt.o --depend=timer_interrupt.d --cpu=Cortex-M3 --apcs=interwork -O0 -IC:\Keil\ARM\CMSIS\Include -IC:\Keil\ARM\INC\NXP\LPC17xx --omf_browse=timer_interrupt.crf TIMER\Timer_interrupt.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||.text||, CODE, READONLY, ALIGN=2

TIMER1_IRQHandler PROC
        PUSH     {r4,lr}
        LDR      r0,|L1.276|
        LDR      r0,[r0,#0]
        TST      r0,#1
        BEQ      |L1.86|
        LDR      r0,|L1.276|
        LDR      r0,[r0,#0]
        ORR      r0,r0,#1
        LDR      r1,|L1.276|
        STR      r0,[r1,#0]
        MOV      r0,r1
        LDR      r0,[r0,#0x18]
        LDR      r1,|L1.280|
        LDR      r1,[r1,#0]  ; general_period_ms
        ADD      r0,r0,r1
        LDR      r1,|L1.276|
        STR      r0,[r1,#0x18]
        LDR      r0,|L1.284|
        LDR      r0,[r0,#0]  ; fps_aux
        LDR      r1,|L1.288|
        STR      r0,[r1,#0]  ; fps
        MOV      r0,#0
        LDR      r1,|L1.284|
        STR      r0,[r1,#0]  ; fps_aux
        LDR      r0,|L1.292|
        LDRB     r0,[r0,#0]  ; time_pong
        ADD      r0,r0,#1
        LDR      r1,|L1.292|
        STRB     r0,[r1,#0]
        LDR      r0,|L1.296|
        LDRH     r0,[r0,#0]  ; time_game
        ADD      r0,r0,#1
        LDR      r1,|L1.296|
        STRH     r0,[r1,#0]
        MOV      r0,#1
        LDR      r1,|L1.300|
        STRB     r0,[r1,#0]
|L1.86|
        LDR      r0,|L1.276|
        LDR      r0,[r0,#0]
        TST      r0,#2
        BEQ      |L1.116|
        LDR      r0,|L1.276|
        LDR      r0,[r0,#0]
        ORR      r0,r0,#2
        LDR      r1,|L1.276|
        STR      r0,[r1,#0]
        MOV      r0,#0
        LDR      r1,|L1.304|
        STRH     r0,[r1,#0]
|L1.116|
        LDR      r0,|L1.276|
        LDR      r0,[r0,#0]
        TST      r0,#4
        BEQ      |L1.152|
        LDR      r0,|L1.276|
        LDR      r0,[r0,#0]
        ORR      r0,r0,#4
        LDR      r1,|L1.276|
        STR      r0,[r1,#0]
        MOV      r0,r1
        LDR      r0,[r0,#0x20]
        LDR      r1,|L1.308|
        LDR      r1,[r1,#0]  ; general_period_us
        ADD      r0,r0,r1
        LDR      r1,|L1.276|
        STR      r0,[r1,#0x20]
|L1.152|
        LDR      r0,|L1.276|
        LDR      r0,[r0,#0]
        TST      r0,#8
        BEQ      |L1.272|
        LDR      r0,|L1.276|
        LDR      r0,[r0,#0]
        ORR      r0,r0,#8
        LDR      r1,|L1.276|
        STR      r0,[r1,#0]
        LDR      r0,|L1.312|
        LDRB     r0,[r0,#0]  ; game
        CMP      r0,#0
        BLE      |L1.198|
        MOV      r0,r1
        LDR      r0,[r0,#0x24]
        LDR      r1,|L1.316|
        LDR      r1,[r1,#0]  ; general_period2_ms
        ADD      r0,r0,r1
        LDR      r1,|L1.276|
        STR      r0,[r1,#0x24]
        B        |L1.210|
|L1.198|
        LDR      r0,|L1.276|
        LDR      r0,[r0,#0x24]
        SUB      r0,r0,#1
        LDR      r1,|L1.276|
        STR      r0,[r1,#0x24]
|L1.210|
        LDR      r0,|L1.320|
        LDRB     r0,[r0,#0]  ; gaming
        CBNZ     r0,|L1.272|
        MOV      r0,#1
        LDR      r1,|L1.320|
        STRB     r0,[r1,#0]
        LDR      r0,|L1.312|
        LDRB     r0,[r0,#0]  ; game
        CMP      r0,#1
        BNE      |L1.238|
        BL       PONG
        B        |L1.264|
|L1.238|
        LDR      r0,|L1.312|
        LDRB     r0,[r0,#0]  ; game
        CMP      r0,#3
        BNE      |L1.252|
        BL       CANDY_CRUSH
        B        |L1.264|
|L1.252|
        LDR      r0,|L1.312|
        LDRB     r0,[r0,#0]  ; game
        CMP      r0,#2
        BNE      |L1.264|
        BL       space_invaders
|L1.264|
        MOV      r0,#0
        LDR      r1,|L1.320|
        STRB     r0,[r1,#0]
|L1.272|
        POP      {r4,pc}
        ENDP

        DCW      0x0000
|L1.276|
        DCD      0x40008000
|L1.280|
        DCD      general_period_ms
|L1.284|
        DCD      fps_aux
|L1.288|
        DCD      ||fps||
|L1.292|
        DCD      time_pong
|L1.296|
        DCD      time_game
|L1.300|
        DCD      move_inv_flag
|L1.304|
        DCD      delay_only
|L1.308|
        DCD      general_period_us
|L1.312|
        DCD      game
|L1.316|
        DCD      general_period2_ms
|L1.320|
        DCD      gaming

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        AREA ||.bss||, DATA, NOINIT, ALIGN=1

delayers
        %        20
cado
        %        10

        AREA ||.data||, DATA, ALIGN=2

gaming
        DCB      0x00,0x00
delay_only
        DCW      0x0000
move_inv_flag
        DCB      0x00,0x00,0x00,0x00
flag
        DCD      0x00000000
i
        DCD      0x00000000

;*** Start embedded assembler ***

#line 1 "TIMER\\Timer_interrupt.c"
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__asm___17_Timer_interrupt_c_gaming____REV16|
#line 114 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cmInstr.h"
|__asm___17_Timer_interrupt_c_gaming____REV16| PROC
#line 115

 rev16 r0, r0
 bx lr
	ENDP
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__asm___17_Timer_interrupt_c_gaming____REVSH|
#line 128
|__asm___17_Timer_interrupt_c_gaming____REVSH| PROC
#line 129

 revsh r0, r0
 bx lr
	ENDP

;*** End   embedded assembler ***

        EXPORT TIMER1_IRQHandler [CODE]
        EXPORT delayers [DATA,SIZE=20]
        EXPORT cado [DATA,SIZE=10]
        EXPORT gaming [DATA,SIZE=1]
        EXPORT delay_only [DATA,SIZE=2]
        EXPORT move_inv_flag [DATA,SIZE=1]
        EXPORT flag [DATA,SIZE=4]
        EXPORT i [DATA,SIZE=4]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT PONG [CODE]
        IMPORT CANDY_CRUSH [CODE]
        IMPORT space_invaders [CODE]
        IMPORT general_period_ms [DATA]
        IMPORT fps_aux [DATA]
        IMPORT ||fps|| [DATA]
        IMPORT time_pong [DATA]
        IMPORT time_game [DATA]
        IMPORT general_period_us [DATA]
        IMPORT game [DATA]
        IMPORT general_period2_ms [DATA]

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
