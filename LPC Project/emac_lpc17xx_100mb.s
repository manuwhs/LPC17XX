; generated by ARM C/C++ Compiler, 4.1 [Build 791]
; commandline ArmCC [--debug -c --asm -oemac_lpc17xx_100mb.o --depend=emac_lpc17xx_100mb.d --cpu=Cortex-M3 --apcs=interwork -O0 -IC:\Keil\ARM\CMSIS\Include -IC:\Keil\ARM\INC\NXP\LPC17xx --omf_browse=emac_lpc17xx_100mb.crf WEBSERVER\Configuration_files\EMAC_LPC17xx_100Mb.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||.text||, CODE, READONLY, ALIGN=2

tx_descr_init PROC
        MOV      r0,#0
        B        |L1.42|
|L1.6|
        ADD      r1,r0,r0,LSL #1
        LDR      r2,|L1.1008|
        ADD      r1,r2,r1,LSL #9
        LDR      r2,|L1.1012|
        STR      r1,[r2,r0,LSL #3]
        MOV      r1,#0
        ADD      r2,r2,r0,LSL #3
        STR      r1,[r2,#4]
        LDR      r2,|L1.1016|
        STR      r1,[r2,r0,LSL #2]
        ADD      r0,r0,#1
|L1.42|
        CMP      r0,#3
        BCC      |L1.6|
        LDR      r1,|L1.1012|
        LDR      r2,|L1.1020|
        STR      r1,[r2,#0]
        LDR      r1,|L1.1016|
        MOV      r2,#0x50000000
        STR      r1,[r2,#0x120]
        MOV      r1,#2
        LDR      r2,|L1.1024|
        STR      r1,[r2,#0]
        MOV      r1,#0
        MOV      r2,#0x50000000
        STR      r1,[r2,#0x128]
        BX       lr
        ENDP

rx_descr_init PROC
        MOV      r0,#0
        B        |L1.134|
|L1.90|
        ADD      r1,r0,r0,LSL #1
        LDR      r2,|L1.1028|
        ADD      r1,r2,r1,LSL #9
        LDR      r2,|L1.1032|
        STR      r1,[r2,r0,LSL #3]
        LDR      r1,|L1.1036|
        ADD      r2,r2,r0,LSL #3
        STR      r1,[r2,#4]
        MOV      r1,#0
        LDR      r2,|L1.1040|
        STR      r1,[r2,r0,LSL #3]
        ADD      r2,r2,r0,LSL #3
        STR      r1,[r2,#4]
        ADD      r0,r0,#1
|L1.134|
        CMP      r0,#4
        BCC      |L1.90|
        LDR      r1,|L1.1032|
        LDR      r2,|L1.1044|
        STR      r1,[r2,#0]
        LDR      r1,|L1.1040|
        MOV      r2,#0x50000000
        STR      r1,[r2,#0x10c]
        MOV      r1,#3
        LDR      r2,|L1.1048|
        STR      r1,[r2,#0]
        MOV      r1,#0
        MOV      r2,#0x50000000
        STR      r1,[r2,#0x118]
        BX       lr
        ENDP

turnaround_MDIO PROC
        LDR      r0,|L1.1052|
        LDR      r0,[r0,#0x40]
        BIC      r0,r0,#0x200
        LDR      r1,|L1.1052|
        STR      r0,[r1,#0x40]
        MOV      r0,#0x100
        STR      r0,[r1,#0x58]
        NOP      
        NOP      
        NOP      
        STR      r0,[r1,#0x5c]
        NOP      
        NOP      
        NOP      
        BX       lr
        ENDP

input_MDIO PROC
        MOV      r0,#0
        MOV      r1,r0
        B        |L1.256|
|L1.218|
        LSL      r0,r0,#1
        MOV      r2,#0x100
        LDR      r3,|L1.1052|
        STR      r2,[r3,#0x58]
        NOP      
        NOP      
        NOP      
        STR      r2,[r3,#0x5c]
        MOV      r2,r3
        LDR      r2,[r2,#0x54]
        TST      r2,#0x200
        BEQ      |L1.252|
        ORR      r0,r0,#1
|L1.252|
        ADD      r1,r1,#1
|L1.256|
        CMP      r1,#0x10
        BCC      |L1.218|
        BX       lr
        ENDP

output_MDIO PROC
        RSB      r2,r1,#0x20
        LSL      r0,r0,r2
        B        |L1.326|
|L1.272|
        TST      r0,#0x80000000
        BEQ      |L1.288|
        MOV      r2,#0x200
        LDR      r3,|L1.1052|
        STR      r2,[r3,#0x58]
        B        |L1.296|
|L1.288|
        MOV      r2,#0x200
        LDR      r3,|L1.1052|
        STR      r2,[r3,#0x5c]
|L1.296|
        NOP      
        NOP      
        NOP      
        MOV      r2,#0x100
        LDR      r3,|L1.1052|
        STR      r2,[r3,#0x58]
        NOP      
        NOP      
        NOP      
        STR      r2,[r3,#0x5c]
        LSL      r0,r0,#1
        SUB      r1,r1,#1
|L1.326|
        CMP      r1,#0
        BNE      |L1.272|
        BX       lr
        ENDP

read_PHY PROC
        PUSH     {r4-r6,lr}
        MOV      r6,r0
        LDR      r0,|L1.1056|
        LDRB     r0,[r0,#0]  ; dev_175x
        CMP      r0,#1
        BNE      |L1.408|
        LDR      r0,|L1.1052|
        LDR      r0,[r0,#0x40]
        ORR      r0,r0,#0x200
        LDR      r1,|L1.1052|
        STR      r0,[r1,#0x40]
        MOV      r1,#0x20
        MOV      r0,#0xffffffff
        BL       output_MDIO
        MOVS     r1,#4
        MOVS     r0,#6
        BL       output_MDIO
        MOVS     r1,#5
        MOVS     r0,#1
        BL       output_MDIO
        MOVS     r1,#5
        MOV      r0,r6
        BL       output_MDIO
        BL       turnaround_MDIO
        BL       input_MDIO
        MOV      r5,r0
        BL       turnaround_MDIO
        B        |L1.470|
|L1.408|
        ORR      r0,r6,#0x100
        MOV      r1,#0x50000000
        STR      r0,[r1,#0x28]
        MOV      r0,#1
        STR      r0,[r1,#0x24]
        MOV      r4,#0
        B        |L1.448|
|L1.430|
        MOV      r0,#0x50000000
        LDR      r0,[r0,#0x34]
        TST      r0,#1
        BNE      |L1.444|
        B        |L1.454|
|L1.444|
        ADD      r4,r4,#1
|L1.448|
        CMP      r4,#0x50000
        BCC      |L1.430|
|L1.454|
        NOP      
        MOV      r0,#0
        MOV      r1,#0x50000000
        STR      r0,[r1,#0x24]
        MOV      r0,r1
        LDR      r5,[r0,#0x30]
|L1.470|
        UXTH     r0,r5
        POP      {r4-r6,pc}
        ENDP

write_PHY PROC
        PUSH     {r4-r6,lr}
        MOV      r6,r0
        MOV      r4,r1
        LDR      r0,|L1.1056|
        LDRB     r0,[r0,#0]  ; dev_175x
        CMP      r0,#1
        BNE      |L1.558|
        LDR      r0,|L1.1052|
        LDR      r0,[r0,#0x40]
        ORR      r0,r0,#0x200
        LDR      r1,|L1.1052|
        STR      r0,[r1,#0x40]
        MOV      r1,#0x20
        MOV      r0,#0xffffffff
        BL       output_MDIO
        MOVS     r1,#4
        MOVS     r0,#5
        BL       output_MDIO
        MOVS     r1,#5
        MOVS     r0,#1
        BL       output_MDIO
        MOVS     r1,#5
        MOV      r0,r6
        BL       output_MDIO
        MOVS     r1,#2
        MOV      r0,r1
        BL       output_MDIO
        MOVS     r1,#0x10
        MOV      r0,r4
        BL       output_MDIO
        BL       turnaround_MDIO
        B        |L1.604|
|L1.558|
        ORR      r0,r6,#0x100
        MOV      r1,#0x50000000
        STR      r0,[r1,#0x28]
        MOV      r0,r1
        STR      r4,[r0,#0x2c]
        MOV      r5,#0
        B        |L1.596|
|L1.578|
        MOV      r0,#0x50000000
        LDR      r0,[r0,#0x34]
        TST      r0,#1
        BNE      |L1.592|
        B        |L1.602|
|L1.592|
        ADD      r5,r5,#1
|L1.596|
        CMP      r5,#0x50000
        BCC      |L1.578|
|L1.602|
        NOP      
|L1.604|
        POP      {r4-r6,pc}
        ENDP

init_ethernet PROC
        PUSH     {r2-r8,lr}
        MOV      r0,#0
        LDR      r1,|L1.1056|
        STRB     r0,[r1,#0]
        MOV      r0,#0x36
        STR      r0,[sp,#0]
        MOV      r1,sp
        MOV      r0,sp
        LDR      r2,|L1.1060|
        LDR      r2,[r2,#0]  ; iap_entry
        BLX      r2
        MOVS     r1,#0x25
        LDR      r0,[sp,#4]
        CMP      r1,r0,LSR #24
        BNE      |L1.652|
        MOV      r0,#1
        LDR      r1,|L1.1056|
        STRB     r0,[r1,#0]
|L1.652|
        LDR      r0,|L1.1064|
        LDR      r0,[r0,#0]
        ORR      r0,r0,#0x40000000
        LDR      r1,|L1.1068|
        STR      r0,[r1,#0xc4]
        LDR      r0,|L1.1072|
        LDR      r1,|L1.1076|
        STR      r0,[r1,#8]
        LDR      r0,|L1.1056|
        LDRB     r0,[r0,#0]  ; dev_175x
        CBNZ     r0,|L1.694|
        MOV      r0,r1
        LDR      r0,[r0,#0xc]
        BIC      r0,r0,#0xf
        ADD      r0,r0,#5
        STR      r0,[r1,#0xc]
        B        |L1.718|
|L1.694|
        LDR      r0,|L1.1076|
        LDR      r0,[r0,#0x10]
        BIC      r0,r0,#0xf0000
        LDR      r1,|L1.1076|
        STR      r0,[r1,#0x10]
        LDR      r0,|L1.1052|
        LDR      r0,[r0,#0x40]
        ORR      r0,r0,#0x100
        LDR      r1,|L1.1052|
        STR      r0,[r1,#0x40]
|L1.718|
        MOV      r0,#0xcf00
        MOV      r1,#0x50000000
        STR      r0,[r1,#0]
        MOV      r0,#0x78
        LDR      r1,|L1.1080|
        STR      r0,[r1,#0]
        MOV      r4,#0
        B        |L1.746|
|L1.742|
        ADD      r4,r4,#1
|L1.746|
        CMP      r4,#0x1000
        BCC      |L1.742|
        MOV      r0,#2
        MOV      r1,#0x50000000
        STR      r0,[r1,#0]
        MOV      r0,#0x30
        STR      r0,[r1,#4]
        MOV      r0,#0x600
        STR      r0,[r1,#0x14]
        MOV      r0,#0x370f
        STR      r0,[r1,#0x10]
        MOV      r0,#0x12
        STR      r0,[r1,#0xc]
        MOV      r0,#0x803c
        STR      r0,[r1,#0x20]
        MOV      r4,#0
        B        |L1.802|
|L1.798|
        ADD      r4,r4,#1
|L1.802|
        CMP      r4,#0x1000
        BCC      |L1.798|
        MOV      r0,#0x3c
        MOV      r1,#0x50000000
        STR      r0,[r1,#0x20]
        MOV      r0,#0x2c0
        LDR      r1,|L1.1080|
        STR      r0,[r1,#0]
        MOV      r0,#0x900
        MOV      r1,#0x50000000
        STR      r0,[r1,#0x18]
        MOV      r4,#0
        B        |L1.846|
|L1.842|
        ADD      r4,r4,#1
|L1.846|
        CMP      r4,#0x1000
        BCC      |L1.842|
        MOV      r0,#0
        MOV      r1,#0x50000000
        STR      r0,[r1,#0x18]
        MOV      r1,#0x8000
        BL       write_PHY
        MOV      r4,#0
        B        |L1.896|
|L1.876|
        MOVS     r0,#0
        BL       read_PHY
        MOV      r5,r0
        TST      r5,#0x8800
        BNE      |L1.892|
        B        |L1.902|
|L1.892|
        ADD      r4,r4,#1
|L1.896|
        CMP      r4,#0x100000
        BCC      |L1.876|
|L1.902|
        NOP      
        MOVS     r0,#2
        BL       read_PHY
        MOV      r7,r0
        MOV      r0,#3
        BL       read_PHY
        MOV      r6,r0
        MOV      r0,#0xfff0
        AND      r0,r6,r0
        ORR      r0,r0,r7,LSL #16
        SUB      r1,r0,#0x7c000
        SUBS     r1,r1,#0xf0
        BNE      |L1.986|
        MOV      r1,#0x3000
        MOVS     r0,#0
        BL       write_PHY
        MOV      r4,#0
        B        |L1.978|
|L1.958|
        MOVS     r0,#1
        BL       read_PHY
        MOV      r5,r0
        TST      r5,#0x20
        BEQ      |L1.974|
        B        |L1.984|
|L1.974|
        ADD      r4,r4,#1
|L1.978|
        CMP      r4,#0x100000
        BCC      |L1.958|
|L1.984|
        NOP      
|L1.986|
        MOV      r4,#0
        B        |L1.1088|
|L1.992|
        MOVS     r0,#0x10
        BL       read_PHY
        MOV      r5,r0
        TST      r5,#4
        BEQ      |L1.1084|
        B        |L1.1094|
|L1.1008|
        DCD      tx_buf
|L1.1012|
        DCD      Tx_Desc
|L1.1016|
        DCD      Tx_Stat
|L1.1020|
        DCD      0x5000011c
|L1.1024|
        DCD      0x50000124
|L1.1028|
        DCD      rx_buf
|L1.1032|
        DCD      Rx_Desc
|L1.1036|
        DCD      0x800005ff
|L1.1040|
        DCD      Rx_Stat
|L1.1044|
        DCD      0x50000108
|L1.1048|
        DCD      0x50000110
|L1.1052|
        DCD      0x2009c000
|L1.1056|
        DCD      dev_175x
|L1.1060|
        DCD      iap_entry
|L1.1064|
        DCD      0x400fc0c4
|L1.1068|
        DCD      0x400fc000
|L1.1072|
        DCD      0x50150105
|L1.1076|
        DCD      0x4002c000
|L1.1080|
        DCD      0x50000100
|L1.1084|
        ADD      r4,r4,#1
|L1.1088|
        CMP      r4,#0x10000
        BCC      |L1.992|
|L1.1094|
        NOP      
        TST      r5,#4
        BEQ      |L1.1138|
        MOV      r0,#0x50000000
        LDR      r0,[r0,#4]
        ORR      r0,r0,#1
        MOV      r1,#0x50000000
        STR      r0,[r1,#4]
        LDR      r0,|L1.1596|
        LDR      r0,[r0,#0]
        ORR      r0,r0,#0x400
        STR      r0,[r1,#0x100]
        MOV      r0,#0x15
        STR      r0,[r1,#8]
        B        |L1.1148|
|L1.1138|
        MOV      r0,#0x12
        MOV      r1,#0x50000000
        STR      r0,[r1,#8]
|L1.1148|
        TST      r5,#2
        BEQ      |L1.1166|
        MOV      r0,#0
        MOV      r1,#0x50000000
        STR      r0,[r1,#0x18]
        B        |L1.1176|
|L1.1166|
        MOV      r0,#0x100
        MOV      r1,#0x50000000
        STR      r0,[r1,#0x18]
|L1.1176|
        LDR      r0,|L1.1600|
        LDRB     r0,[r0,#4]  ; own_hw_adr
        LDR      r1,|L1.1600|
        LDRB     r1,[r1,#5]  ; own_hw_adr
        ORR      r0,r0,r1,LSL #8
        MOV      r1,#0x50000000
        STR      r0,[r1,#0x40]
        LDR      r0,|L1.1600|
        LDRB     r0,[r0,#2]  ; own_hw_adr
        LDR      r1,|L1.1600|
        LDRB     r1,[r1,#3]  ; own_hw_adr
        ORR      r0,r0,r1,LSL #8
        MOV      r1,#0x50000000
        STR      r0,[r1,#0x44]
        LDR      r0,|L1.1600|
        LDRB     r0,[r0,#0]  ; own_hw_adr
        LDR      r1,|L1.1600|
        LDRB     r1,[r1,#1]  ; own_hw_adr
        ORR      r0,r0,r1,LSL #8
        MOV      r1,#0x50000000
        STR      r0,[r1,#0x48]
        BL       rx_descr_init
        BL       tx_descr_init
        MOV      r0,#0x26
        LDR      r1,|L1.1604|
        STR      r0,[r1,#0]
        MOV      r0,#0x88
        LDR      r1,|L1.1608|
        STR      r0,[r1,#0]
        MOV      r0,#0xffff
        ADD      r1,r1,#4
        STR      r0,[r1,#0]
        MOV      r0,#0x50000000
        LDR      r0,[r0,#0x100]
        ORR      r0,r0,#3
        MOV      r1,#0x50000000
        STR      r0,[r1,#0x100]
        MOV      r0,r1
        LDR      r0,[r0,#0]
        ORR      r0,r0,#1
        STR      r0,[r1,#0]
        POP      {r2-r8,pc}
        ENDP

int_enable_eth PROC
        MOV      r0,#0x1c
        MOV      r1,#1
        LSL      r1,r1,r0
        LSR      r2,r0,#5
        MOV      r3,#0xe000e000
        ADD      r2,r3,r2,LSL #2
        STR      r1,[r2,#0x100]
        NOP      
        BX       lr
        ENDP

int_disable_eth PROC
        MOV      r0,#0x1c
        MOV      r1,#1
        LSL      r1,r1,r0
        LDR      r2,|L1.1612|
        LSR      r3,r0,#5
        STR      r1,[r2,r3,LSL #2]
        NOP      
        BX       lr
        ENDP

send_frame PROC
        PUSH     {r4-r6,lr}
        LDR      r5,|L1.1616|
        LDR      r1,[r5,#0]
        ADD      r3,r0,#4
        LDR      r5,|L1.1620|
        LDR      r4,[r5,r1,LSL #3]
        LDRH     r5,[r0,#0]
        ADD      r5,r5,#3
        ASR      r2,r5,#2
        B        |L1.1392|
|L1.1384|
        LDM      r3!,{r5}
        STM      r4!,{r5}
        SUB      r2,r2,#1
|L1.1392|
        CMP      r2,#0
        BNE      |L1.1384|
        LDRH     r5,[r0,#0]
        SUB      r5,r5,#1
        ORR      r5,r5,#0xc0000000
        LDR      r6,|L1.1620|
        ADD      r6,r6,r1,LSL #3
        STR      r5,[r6,#4]
        ADD      r5,r1,#1
        MOV      r1,r5
        CMP      r5,#3
        BNE      |L1.1426|
        MOV      r1,r2
|L1.1426|
        LDR      r5,|L1.1616|
        STR      r1,[r5,#0]
        POP      {r4-r6,pc}
        ENDP

ENET_IRQHandler PROC
        PUSH     {r4-r10,lr}
        B        |L1.1576|
|L1.1438|
        LDR      r0,|L1.1624|
        STR      r8,[r0,#0]
        TST      r8,#8
        BEQ      |L1.1574|
        LDR      r0,|L1.1628|
        LDR      r4,[r0,#0]
        B        |L1.1566|
|L1.1456|
        LDR      r0,|L1.1632|
        LDR      r7,[r0,r4,LSL #3]
        TST      r7,#0x40000000
        BNE      |L1.1470|
        B        |L1.1550|
|L1.1470|
        UBFX     r0,r7,#0,#11
        SUB      r5,r0,#3
        MOV      r0,#0x5ea
        CMP      r5,r0
        BHI      |L1.1492|
        LDR      r0,|L1.1636|
        TST      r7,r0
        BEQ      |L1.1494|
|L1.1492|
        B        |L1.1550|
|L1.1494|
        ORR      r0,r5,#0x80000000
        BL       alloc_mem
        MOV      r6,r0
        CBZ      r6,|L1.1548|
        ADD      r10,r6,#4
        LDR      r0,|L1.1640|
        LDR      r9,[r0,r4,LSL #3]
        ADD      r0,r5,#3
        LSR      r5,r0,#2
        B        |L1.1538|
|L1.1526|
        LDR      r0,[r9],#4
        STR      r0,[r10],#4
        SUB      r5,r5,#1
|L1.1538|
        CMP      r5,#0
        BNE      |L1.1526|
        MOV      r0,r6
        BL       put_in_queue
|L1.1548|
        NOP      
|L1.1550|
        ADDS     r0,r4,#1
        MOV      r4,r0
        CMP      r0,#4
        BNE      |L1.1562|
        MOV      r4,#0
|L1.1562|
        LDR      r0,|L1.1628|
        STR      r4,[r0,#0]
|L1.1566|
        LDR      r0,|L1.1644|
        LDR      r0,[r0,#0]
        CMP      r0,r4
        BNE      |L1.1456|
|L1.1574|
        NOP      
|L1.1576|
        LDR      r0,|L1.1608|
        SUBS     r0,r0,#4
        LDR      r0,[r0,#0]
        LDR      r1,|L1.1608|
        LDR      r1,[r1,#0]
        ANDS     r8,r0,r1
        BNE      |L1.1438|
        POP      {r4-r10,pc}
        ENDP

|L1.1596|
        DCD      0x50000100
|L1.1600|
        DCD      own_hw_adr
|L1.1604|
        DCD      0x50000200
|L1.1608|
        DCD      0x50000fe4
|L1.1612|
        DCD      0xe000e180
|L1.1616|
        DCD      0x50000128
|L1.1620|
        DCD      Tx_Desc
|L1.1624|
        DCD      0x50000fe8
|L1.1628|
        DCD      0x50000118
|L1.1632|
        DCD      Rx_Stat
|L1.1636|
        DCD      0x1b900000
|L1.1640|
        DCD      Rx_Desc
|L1.1644|
        DCD      0x50000114

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        AREA ||.bss||, DATA, NOINIT, ALIGN=3

Rx_Desc
        %        32
Rx_Stat
        %        32
Tx_Desc
        %        24
Tx_Stat
        %        12
rx_buf
        %        6144
tx_buf
        %        4608

        AREA ||.data||, DATA, ALIGN=2

iap_entry
        DCD      0x1fff1ff1
dev_175x
        DCB      0x00

;*** Start embedded assembler ***

#line 1 "WEBSERVER\\Configuration_files\\EMAC_LPC17xx_100Mb.c"
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__asm___20_EMAC_LPC17xx_100Mb_c_f63b8310____REV16|
#line 114 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cmInstr.h"
|__asm___20_EMAC_LPC17xx_100Mb_c_f63b8310____REV16| PROC
#line 115

 rev16 r0, r0
 bx lr
	ENDP
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__asm___20_EMAC_LPC17xx_100Mb_c_f63b8310____REVSH|
#line 128
|__asm___20_EMAC_LPC17xx_100Mb_c_f63b8310____REVSH| PROC
#line 129

 revsh r0, r0
 bx lr
	ENDP

;*** End   embedded assembler ***

        EXPORT init_ethernet [CODE]
        EXPORT int_enable_eth [CODE]
        EXPORT int_disable_eth [CODE]
        EXPORT send_frame [CODE]
        EXPORT ENET_IRQHandler [CODE]
        EXPORT iap_entry [DATA,SIZE=4]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT own_hw_adr [DATA]
        IMPORT alloc_mem [CODE]
        IMPORT put_in_queue [CODE]

        KEEP tx_descr_init
        KEEP rx_descr_init
        KEEP turnaround_MDIO
        KEEP input_MDIO
        KEEP output_MDIO
        KEEP read_PHY
        KEEP write_PHY
        KEEP Rx_Desc
        KEEP Rx_Stat
        KEEP Tx_Desc
        KEEP Tx_Stat
        KEEP rx_buf
        KEEP tx_buf
        KEEP dev_175x

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