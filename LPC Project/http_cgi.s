; generated by ARM C/C++ Compiler, 4.1 [Build 791]
; commandline ArmCC [--debug -c --asm -ohttp_cgi.o --depend=http_cgi.d --cpu=Cortex-M3 --apcs=interwork -O0 -IC:\Keil\ARM\CMSIS\Include -IC:\Keil\ARM\INC\NXP\LPC17xx --omf_browse=http_cgi.crf WEBSERVER\Configuration_files\HTTP_CGI.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||.text||, CODE, READONLY, ALIGN=2

        REQUIRE _printf_pre_padding
        REQUIRE _printf_truncate_unsigned
        REQUIRE _printf_truncate_signed
        REQUIRE _printf_sizespec
        REQUIRE _printf_percent
        REQUIRE _printf_return_value
        REQUIRE _printf_flags
        REQUIRE _printf_widthprec
        REQUIRE _printf_d
        REQUIRE _printf_i
        REQUIRE _printf_u
        REQUIRE _printf_o
        REQUIRE _printf_x
        REQUIRE _printf_c
        REQUIRE _printf_lc
        REQUIRE _printf_int_dec
        REQUIRE _printf_longlong_oct
        REQUIRE _printf_longlong_hex
        REQUIRE _printf_str
        REQUIRE _printf_wctomb
        REQUIRE _printf_str
        REQUIRE _printf_wctomb
        REQUIRE _printf_str
        REQUIRE _printf_wctomb
        REQUIRE _printf_str
        REQUIRE _printf_wctomb
        REQUIRE _printf_str
        REQUIRE _printf_wctomb
        REQUIRE _printf_str
        REQUIRE _printf_wctomb
        REQUIRE _printf_str
        REQUIRE _printf_wctomb
        REQUIRE _printf_str
        REQUIRE _printf_wctomb
        REQUIRE _printf_str
        REQUIRE _printf_wctomb
        REQUIRE _printf_str
        REQUIRE _printf_wctomb
        REQUIRE _printf_str
        REQUIRE _printf_wctomb
        REQUIRE _printf_str
        REQUIRE _printf_wctomb
        REQUIRE _printf_str
        REQUIRE _printf_wctomb
        REQUIRE _printf_str
        REQUIRE _printf_wctomb
        REQUIRE _printf_str
        REQUIRE _printf_wctomb
        REQUIRE _printf_str
        REQUIRE _printf_wctomb
        REQUIRE _printf_str
        REQUIRE _printf_wctomb
cgi_process_data PROC
        PUSH     {r4-r10,lr}
        SUB      sp,sp,#0x18
        MOV      r6,r0
        MOV      r5,r1
        MOV      r7,r2
        MOV      r0,#0
        STRB     r0,[r5,r7]
        CBNZ     r6,|L1.22|
        B        |L1.28|
|L1.22|
        ADD      sp,sp,#0x18
        POP      {r4-r10,pc}
|L1.28|
        NOP      
        MOV      r8,#0
        MOV      r0,#0x28
        BL       alloc_mem
        MOV      r4,r0
        NOP      
|L1.46|
        MOVS     r2,#0x28
        MOV      r1,r4
        MOV      r0,r5
        BL       http_get_var
        MOV      r5,r0
        LDRB     r0,[r4,#0]
        CMP      r0,#0
        BEQ      |L1.308|
        ADR      r1,|L1.1080|
        MOV      r0,r4
        BL       str_scomp
        CMP      r0,#1
        BNE      |L1.98|
        ADD      r1,r4,#0xc
        LDR      r0,|L1.1096|
        BL       str_copy
        LDR      r0,|L1.1096|
        BL       conversor_SaI
        LDR      r1,|L1.1100|
        STRB     r0,[r1,#0]
        B        |L1.22|
|L1.98|
        LDR      r0,|L1.1100|
        LDRB     r0,[r0,#0]  ; select_menu
        CMP      r0,#1
        BNE      |L1.162|
        ADR      r1,|L1.1104|
        MOV      r0,r4
        BL       str_scomp
        CMP      r0,#1
        BNE      |L1.164|
        ADDS     r1,r4,#4
        LDR      r0,|L1.1096|
        BL       str_copy
        LDR      r0,|L1.1096|
        BL       conversor_SaI
        LDR      r1,|L1.1112|
        STRB     r0,[r1,#0]
        LDR      r0,|L1.1116|
        LDRB     r0,[r0,#0]  ; OV7670fps
        LDRB     r1,[r1,#0]  ; aux_data_form
        CMP      r0,r1
        BEQ      |L1.162|
        LDR      r0,|L1.1112|
        LDRB     r0,[r0,#0]  ; aux_data_form
        LDR      r1,|L1.1116|
        STRB     r0,[r1,#0]
        MOV      r0,r1
        LDRB     r0,[r0,#0]  ; OV7670fps
        BL       OV7670_FPS
|L1.162|
        B        |L1.760|
|L1.164|
        ADR      r1,|L1.1120|
        MOV      r0,r4
        BL       str_scomp
        CMP      r0,#1
        BNE      |L1.236|
        ADD      r1,r4,#0xb
        LDR      r0,|L1.1096|
        BL       str_copy
        LDR      r0,|L1.1096|
        BL       conversor_SaI
        LDR      r1,|L1.1112|
        STRB     r0,[r1,#0]
        LDR      r0,|L1.1132|
        LDRB     r0,[r0,#0]  ; OV7670contrast
        LDRB     r1,[r1,#0]  ; aux_data_form
        CMP      r0,r1
        BEQ      |L1.162|
        LDR      r0,|L1.1112|
        LDRB     r0,[r0,#0]  ; aux_data_form
        LDR      r1,|L1.1132|
        STRB     r0,[r1,#0]
        LDR      r0,|L1.1136|
        LDRB     r3,[r0,#0]  ; BCL
        LDR      r0,|L1.1140|
        LDRH     r2,[r0,#0]  ; OV7670luminis
        MOV      r0,r1
        LDRB     r1,[r0,#0]  ; OV7670contrast
        LDR      r0,|L1.1144|
        LDRB     r0,[r0,#0]  ; OV7670bright
        BL       OV7670_BriConLum
|L1.234|
        B        |L1.760|
|L1.236|
        ADR      r1,|L1.1148|
        MOV      r0,r4
        BL       str_scomp
        CMP      r0,#1
        BNE      |L1.310|
        ADD      r1,r4,#0xb
        LDR      r0,|L1.1096|
        BL       str_copy
        LDR      r0,|L1.1096|
        BL       conversor_SaI
        LDR      r1,|L1.1112|
        STRB     r0,[r1,#0]
        LDR      r0,|L1.1144|
        LDRB     r0,[r0,#0]  ; OV7670bright
        LDRB     r1,[r1,#0]  ; aux_data_form
        CMP      r0,r1
        BEQ      |L1.162|
        LDR      r0,|L1.1112|
        LDRB     r0,[r0,#0]  ; aux_data_form
        LDR      r1,|L1.1144|
        STRB     r0,[r1,#0]
        LDR      r0,|L1.1136|
        LDRB     r3,[r0,#0]  ; BCL
        LDR      r0,|L1.1140|
        LDRH     r2,[r0,#0]  ; OV7670luminis
        LDR      r0,|L1.1132|
        LDRB     r1,[r0,#0]  ; OV7670contrast
        LDR      r0,|L1.1144|
        LDRB     r0,[r0,#0]  ; OV7670bright
        BL       OV7670_BriConLum
|L1.306|
        B        |L1.760|
|L1.308|
        B        |L1.852|
|L1.310|
        ADR      r1,|L1.1160|
        MOV      r0,r4
        BL       str_scomp
        CMP      r0,#1
        BNE      |L1.382|
        ADD      r1,r4,#0xb
        LDR      r0,|L1.1096|
        BL       str_copy
        LDR      r0,|L1.1096|
        BL       conversor_SaI
        LDR      r1,|L1.1112|
        STRB     r0,[r1,#0]
        LDR      r0,|L1.1140|
        LDRH     r0,[r0,#0]  ; OV7670luminis
        LDRB     r1,[r1,#0]  ; aux_data_form
        CMP      r0,r1
        BEQ      |L1.162|
        LDR      r0,|L1.1112|
        LDRB     r0,[r0,#0]  ; aux_data_form
        LDR      r1,|L1.1140|
        STRH     r0,[r1,#0]
        LDR      r0,|L1.1136|
        LDRB     r3,[r0,#0]  ; BCL
        MOV      r0,r1
        LDRH     r2,[r0,#0]  ; OV7670luminis
        LDR      r0,|L1.1132|
        LDRB     r1,[r0,#0]  ; OV7670contrast
        LDR      r0,|L1.1144|
        LDRB     r0,[r0,#0]  ; OV7670bright
        BL       OV7670_BriConLum
        B        |L1.760|
|L1.382|
        ADR      r1,|L1.1172|
        MOV      r0,r4
        BL       str_scomp
        CMP      r0,#1
        BNE      |L1.450|
        ADD      r1,r4,#0xb
        LDR      r0,|L1.1096|
        BL       str_copy
        LDR      r0,|L1.1096|
        BL       conversor_SaI
        LDR      r1,|L1.1112|
        STRB     r0,[r1,#0]
        LDR      r0,|L1.1184|
        LDRB     r0,[r0,#0]  ; OV7670R_gain
        LDRB     r1,[r1,#0]  ; aux_data_form
        CMP      r0,r1
        BEQ      |L1.234|
        LDR      r0,|L1.1112|
        LDRB     r0,[r0,#0]  ; aux_data_form
        LDR      r1,|L1.1184|
        STRB     r0,[r1,#0]
        LDR      r0,|L1.1188|
        LDRB     r2,[r0,#0]  ; OV7670B_gain
        LDR      r0,|L1.1192|
        LDRB     r1,[r0,#0]  ; OV7670G_gain
        LDR      r0,|L1.1184|
        LDRB     r0,[r0,#0]  ; OV7670R_gain
        BL       OV7670_RGBgain
        B        |L1.760|
|L1.450|
        ADR      r1,|L1.1196|
        MOV      r0,r4
        BL       str_scomp
        CMP      r0,#1
        BNE      |L1.518|
        ADD      r1,r4,#0xb
        LDR      r0,|L1.1096|
        BL       str_copy
        LDR      r0,|L1.1096|
        BL       conversor_SaI
        LDR      r1,|L1.1112|
        STRB     r0,[r1,#0]
        LDR      r0,|L1.1192|
        LDRB     r0,[r0,#0]  ; OV7670G_gain
        LDRB     r1,[r1,#0]  ; aux_data_form
        CMP      r0,r1
        BEQ      |L1.306|
        LDR      r0,|L1.1112|
        LDRB     r0,[r0,#0]  ; aux_data_form
        LDR      r1,|L1.1192|
        STRB     r0,[r1,#0]
        LDR      r0,|L1.1188|
        LDRB     r2,[r0,#0]  ; OV7670B_gain
        MOV      r0,r1
        LDRB     r1,[r0,#0]  ; OV7670G_gain
        LDR      r0,|L1.1184|
        LDRB     r0,[r0,#0]  ; OV7670R_gain
        BL       OV7670_RGBgain
        B        |L1.760|
|L1.518|
        ADR      r1,|L1.1208|
        MOV      r0,r4
        BL       str_scomp
        CMP      r0,#1
        BNE      |L1.586|
        ADD      r1,r4,#0xb
        LDR      r0,|L1.1096|
        BL       str_copy
        LDR      r0,|L1.1096|
        BL       conversor_SaI
        LDR      r1,|L1.1112|
        STRB     r0,[r1,#0]
        LDR      r0,|L1.1188|
        LDRB     r0,[r0,#0]  ; OV7670B_gain
        LDRB     r1,[r1,#0]  ; aux_data_form
        CMP      r0,r1
        BEQ      |L1.760|
        LDR      r0,|L1.1112|
        LDRB     r0,[r0,#0]  ; aux_data_form
        LDR      r1,|L1.1188|
        STRB     r0,[r1,#0]
        MOV      r0,r1
        LDRB     r2,[r0,#0]  ; OV7670B_gain
        LDR      r0,|L1.1192|
        LDRB     r1,[r0,#0]  ; OV7670G_gain
        LDR      r0,|L1.1184|
        LDRB     r0,[r0,#0]  ; OV7670R_gain
        BL       OV7670_RGBgain
        B        |L1.760|
|L1.586|
        ADR      r1,|L1.1220|
        MOV      r0,r4
        BL       str_scomp
        CMP      r0,#1
        BNE      |L1.634|
        ADD      r1,r4,#0xb
        LDR      r0,|L1.1096|
        BL       str_copy
        LDR      r0,|L1.1096|
        BL       conversor_SaI
        MOV      r9,r0
        ASR      r0,r0,#31
        ADD      r0,r9,r0,LSR #29
        UBFX     r0,r0,#3,#8
        LDR      r1,|L1.1232|
        STRB     r0,[r1,#0]
        B        |L1.760|
|L1.634|
        ADR      r1,|L1.1236|
        MOV      r0,r4
        BL       str_scomp
        CMP      r0,#1
        BNE      |L1.692|
        ADDS     r1,r4,#4
        LDR      r0,|L1.1096|
        BL       str_copy
        LDR      r0,|L1.1096|
        BL       conversor_SaI
        LDR      r1,|L1.1112|
        STRB     r0,[r1,#0]
        LDR      r0,|L1.1244|
        LDRB     r0,[r0,#0]  ; RGB
        LDRB     r1,[r1,#0]  ; aux_data_form
        CMP      r0,r1
        BEQ      |L1.760|
        LDR      r0,|L1.1112|
        LDRB     r0,[r0,#0]  ; aux_data_form
        LDR      r1,|L1.1244|
        STRB     r0,[r1,#0]
        MOV      r0,r1
        LDRB     r0,[r0,#0]  ; RGB
        BL       OV7670_AWBmode
        B        |L1.760|
|L1.692|
        ADR      r1,|L1.1248|
        MOV      r0,r4
        BL       str_scomp
        CMP      r0,#1
        BNE      |L1.760|
        ADDS     r1,r4,#4
        LDR      r0,|L1.1096|
        BL       str_copy
        LDR      r0,|L1.1096|
        BL       conversor_SaI
        LDR      r1,|L1.1112|
        STRB     r0,[r1,#0]
        LDR      r0,|L1.1136|
        LDRB     r0,[r0,#0]  ; BCL
        LDRB     r1,[r1,#0]  ; aux_data_form
        CMP      r0,r1
        BEQ      |L1.760|
        LDR      r0,|L1.1112|
        LDRB     r0,[r0,#0]  ; aux_data_form
        LDR      r1,|L1.1136|
        STRB     r0,[r1,#0]
        MOV      r0,r1
        LDRB     r3,[r0,#0]  ; BCL
        LDR      r0,|L1.1140|
        LDRH     r2,[r0,#0]  ; OV7670luminis
        LDR      r0,|L1.1132|
        LDRB     r1,[r0,#0]  ; OV7670contrast
        LDR      r0,|L1.1144|
        LDRB     r0,[r0,#0]  ; OV7670bright
        BL       OV7670_BriConLum
|L1.760|
        LDR      r0,|L1.1100|
        LDRB     r0,[r0,#0]  ; select_menu
        CMP      r0,#0
        BNE      |L1.900|
        ADR      r1,|L1.1256|
        MOV      r0,r4
        BL       str_scomp
        CMP      r0,#1
        BNE      |L1.798|
        ADDS     r1,r4,#4
        LDR      r0,|L1.1096|
        BL       str_copy
        LDR      r0,|L1.1096|
        BL       conversor_SaI
        LDR      r1,|L1.1264|
        STRB     r0,[r1,#0]
|L1.798|
        LDR      r0,|L1.1264|
        LDRB     r0,[r0,#0]  ; modo
        CMP      r0,#1
        BNE      |L1.900|
        ADR      r1,|L1.1196|
        MOV      r0,r4
        BL       str_scomp
        CMP      r0,#1
        BNE      |L1.854|
        ADD      r1,r4,#0xb
        LDR      r0,|L1.1096|
        BL       str_copy
        LDR      r0,|L1.1096|
        BL       conversor_SaI
        MOV      r10,r0
        BL       __aeabi_i2f
        MOV      r9,r0
        LDR      r1,|L1.1268|
        BL       __aeabi_fdiv
        LDR      r1,|L1.1272|
        STR      r0,[r1,#0]  ; umbral_x
|L1.852|
        B        |L1.900|
|L1.854|
        ADR      r1,|L1.1172|
        MOV      r0,r4
        BL       str_scomp
        CMP      r0,#1
        BNE      |L1.900|
        ADD      r1,r4,#0xb
        LDR      r0,|L1.1096|
        BL       str_copy
        LDR      r0,|L1.1096|
        BL       conversor_SaI
        MOV      r10,r0
        BL       __aeabi_i2f
        MOV      r9,r0
        LDR      r1,|L1.1268|
        BL       __aeabi_fdiv
        LDR      r1,|L1.1276|
        STR      r0,[r1,#0]  ; umbral_y
|L1.900|
        CMP      r5,#0
        BNE      |L1.46|
        MOV      r0,r4
        BL       free_mem
        CMP      r8,#3
        BNE      |L1.950|
        ADD      r0,sp,#0xc
        BL       strlen
        UXTH     r7,r0
        MOV      r2,r7
        MOV      r1,sp
        ADD      r0,sp,#0xc
        BL       mem_comp
        CMP      r0,#1
        BNE      |L1.950|
        LDR      r1,|L1.1280|
        LDR      r0,[r1,#0x14]  ; http_config
        ADD      r1,sp,#0xc
        BL       str_copy
|L1.950|
        NOP      
        B        |L1.22|
        ENDP

cgi_func PROC
        PUSH     {r4-r10,lr}
        MOV      r4,r0
        MOV      r5,r1
        MOV      r8,r2
        MOV      r9,r3
        MOV      r6,#0
        LDR      r0,|L1.1100|
        LDRB     r0,[r0,#0]  ; select_menu
        CMP      r0,#0
        BNE      |L1.1076|
        LDRB     r0,[r4,#0]
        SUBS     r0,r0,#0x61
        CMP      r0,#6
        BCS      |L1.1010|
        TBB      [pc,r0]
        DCB      0x03,0x0b
        DCB      0x13,0x1b,0x97,0xa7
        LDR      r0,|L1.1264|
        LDRB     r2,[r0,#0]  ; modo
        ADDS     r1,r4,#2
        MOV      r0,r5
        BL       __2sprintf
        MOV      r6,r0
|L1.1010|
        B        |L1.1340|
        LDR      r0,|L1.1284|
        ADDS     r1,r4,#2
        LDR      r2,[r0,#0]  ; det_x
        MOV      r0,r5
        BL       __2sprintf
        MOV      r6,r0
        B        |L1.1340|
        LDR      r0,|L1.1288|
        ADDS     r1,r4,#2
        LDR      r2,[r0,#0]  ; det_y
        MOV      r0,r5
        BL       __2sprintf
        MOV      r6,r0
        B        |L1.1340|
        LDR      r1,|L1.1268|
        LDR      r0,|L1.1272|
        LDR      r0,[r0,#0]  ; umbral_x
        BL       __aeabi_fmul
        MOV      r10,r0
        BL       __aeabi_f2uiz
        UXTB     r7,r0
        MOV      r2,r7
        ADDS     r1,r4,#2
        MOV      r0,r5
        BL       __2sprintf
        MOV      r6,r0
        B        |L1.1340|
|L1.1076|
        B        |L1.1342|
        DCW      0x0000
|L1.1080|
        DCB      "menu_change=",0
        DCB      0
        DCB      0
        DCB      0
|L1.1096|
        DCD      aux_web
|L1.1100|
        DCD      select_menu
|L1.1104|
        DCB      "fps=",0
        DCB      0
        DCB      0
        DCB      0
|L1.1112|
        DCD      aux_data_form
|L1.1116|
        DCD      OV7670fps
|L1.1120|
        DCB      "barra_inp4=",0
|L1.1132|
        DCD      OV7670contrast
|L1.1136|
        DCD      BCL
|L1.1140|
        DCD      OV7670luminis
|L1.1144|
        DCD      OV7670bright
|L1.1148|
        DCB      "barra_inp3=",0
|L1.1160|
        DCB      "barra_inp5=",0
|L1.1172|
        DCB      "barra_inp0=",0
|L1.1184|
        DCD      OV7670R_gain
|L1.1188|
        DCD      OV7670B_gain
|L1.1192|
        DCD      OV7670G_gain
|L1.1196|
        DCB      "barra_inp1=",0
|L1.1208|
        DCB      "barra_inp2=",0
|L1.1220|
        DCB      "barra_inp6=",0
|L1.1232|
        DCD      umbral
|L1.1236|
        DCB      "RGB=",0
        DCB      0
        DCB      0
        DCB      0
|L1.1244|
        DCD      ||RGB||
|L1.1248|
        DCB      "BCL=",0
        DCB      0
        DCB      0
        DCB      0
|L1.1256|
        DCB      "seg=",0
        DCB      0
        DCB      0
        DCB      0
|L1.1264|
        DCD      modo
|L1.1268|
        DCD      0x437f0000
|L1.1272|
        DCD      umbral_x
|L1.1276|
        DCD      umbral_y
|L1.1280|
        DCD      http_config
|L1.1284|
        DCD      det_x
|L1.1288|
        DCD      det_y
        LDR      r1,|L1.1560|
        LDR      r0,|L1.1564|
        LDR      r0,[r0,#0]  ; umbral_y
        BL       __aeabi_fmul
        MOV      r10,r0
        BL       __aeabi_f2uiz
        UXTB     r7,r0
        MOV      r2,r7
        ADDS     r1,r4,#2
        MOV      r0,r5
        BL       __2sprintf
        MOV      r6,r0
        B        |L1.1340|
        LDR      r0,|L1.1568|
        LDRB     r2,[r0,#0]  ; select_menu
        ADDS     r1,r4,#2
        MOV      r0,r5
        BL       __2sprintf
        MOV      r6,r0
        NOP      
|L1.1340|
        NOP      
|L1.1342|
        LDR      r0,|L1.1568|
        LDRB     r0,[r0,#0]  ; select_menu
        CMP      r0,#1
        BNE      |L1.1554|
        LDRB     r0,[r4,#0]
        SUBS     r0,r0,#0x64
        CMP      r0,#0xb
        BCS      |L1.1552|
        TBB      [pc,r0]
        DCB      0x1e,0x26
        DCB      0x2e,0x06,0x0e,0x16
        DCB      0x36,0x3e,0x46,0x57
        DCB      0x4f,0x00
        LDR      r0,|L1.1572|
        LDRB     r2,[r0,#0]  ; OV7670fps
        ADDS     r1,r4,#2
        MOV      r0,r5
        BL       __2sprintf
        MOV      r6,r0
        B        |L1.1552|
        LDR      r0,|L1.1576|
        LDRB     r2,[r0,#0]  ; OV7670contrast
        ADDS     r1,r4,#2
        MOV      r0,r5
        BL       __2sprintf
        MOV      r6,r0
        B        |L1.1552|
        LDR      r0,|L1.1580|
        LDRB     r2,[r0,#0]  ; OV7670bright
        ADDS     r1,r4,#2
        MOV      r0,r5
        BL       __2sprintf
        MOV      r6,r0
        B        |L1.1552|
        LDR      r0,|L1.1584|
        LDRB     r2,[r0,#0]  ; OV7670G_gain
        ADDS     r1,r4,#2
        MOV      r0,r5
        BL       __2sprintf
        MOV      r6,r0
        B        |L1.1552|
        LDR      r0,|L1.1588|
        LDRB     r2,[r0,#0]  ; OV7670R_gain
        ADDS     r1,r4,#2
        MOV      r0,r5
        BL       __2sprintf
        MOV      r6,r0
        B        |L1.1552|
        LDR      r0,|L1.1568|
        LDRB     r2,[r0,#0]  ; select_menu
        ADDS     r1,r4,#2
        MOV      r0,r5
        BL       __2sprintf
        MOV      r6,r0
        B        |L1.1552|
        LDR      r0,|L1.1592|
        LDRB     r2,[r0,#0]  ; OV7670B_gain
        ADDS     r1,r4,#2
        MOV      r0,r5
        BL       __2sprintf
        MOV      r6,r0
        B        |L1.1552|
        LDR      r0,|L1.1596|
        LDRH     r2,[r0,#0]  ; OV7670luminis
        ADDS     r1,r4,#2
        MOV      r0,r5
        BL       __2sprintf
        MOV      r6,r0
        B        |L1.1552|
        LDR      r0,|L1.1600|
        LDRB     r0,[r0,#0]  ; umbral
        LSLS     r2,r0,#3
        ADDS     r1,r4,#2
        MOV      r0,r5
        BL       __2sprintf
        MOV      r6,r0
        B        |L1.1552|
        LDR      r0,|L1.1604|
        LDRB     r2,[r0,#0]  ; RGB
        ADDS     r1,r4,#2
        MOV      r0,r5
        BL       __2sprintf
        MOV      r6,r0
        B        |L1.1552|
        LDR      r0,|L1.1608|
        LDRB     r2,[r0,#0]  ; BCL
        ADDS     r1,r4,#2
        MOV      r0,r5
        BL       __2sprintf
        MOV      r6,r0
        NOP      
|L1.1552|
        NOP      
|L1.1554|
        UXTH     r0,r6
        POP      {r4-r10,pc}
        ENDP

|L1.1560|
        DCD      0x437f0000
|L1.1564|
        DCD      umbral_y
|L1.1568|
        DCD      select_menu
|L1.1572|
        DCD      OV7670fps
|L1.1576|
        DCD      OV7670contrast
|L1.1580|
        DCD      OV7670bright
|L1.1584|
        DCD      OV7670G_gain
|L1.1588|
        DCD      OV7670R_gain
|L1.1592|
        DCD      OV7670B_gain
|L1.1596|
        DCD      OV7670luminis
|L1.1600|
        DCD      umbral
|L1.1604|
        DCD      ||RGB||
|L1.1608|
        DCD      BCL

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        AREA ||.bss||, DATA, NOINIT, ALIGN=0

aux_web
        %        40

        AREA ||.data||, DATA, ALIGN=2

select_menu
        DCB      0x00
BCL
        DCB      0x01
||RGB||
        DCB      0x01,0x00
adv
        DCD      0x00000000
||P2||
        DCB      0x00
aux_data_form
        DCB      0x00

;*** Start embedded assembler ***

#line 1 "WEBSERVER\\Configuration_files\\HTTP_CGI.c"
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__asm___10_HTTP_CGI_c_d53a327f____REV16|
#line 114 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cmInstr.h"
|__asm___10_HTTP_CGI_c_d53a327f____REV16| PROC
#line 115

 rev16 r0, r0
 bx lr
	ENDP
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__asm___10_HTTP_CGI_c_d53a327f____REVSH|
#line 128
|__asm___10_HTTP_CGI_c_d53a327f____REVSH| PROC
#line 129

 revsh r0, r0
 bx lr
	ENDP

;*** End   embedded assembler ***

        EXPORT cgi_process_data [CODE]
        EXPORT cgi_func [CODE]
        EXPORT aux_web [DATA,SIZE=40]
        EXPORT select_menu [DATA,SIZE=1]
        EXPORT BCL [DATA,SIZE=1]
        EXPORT ||RGB|| [DATA,SIZE=1]
        EXPORT aux_data_form [DATA,SIZE=1]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT alloc_mem [CODE]
        IMPORT http_get_var [CODE]
        IMPORT str_scomp [CODE]
        IMPORT str_copy [CODE]
        IMPORT conversor_SaI [CODE]
        IMPORT OV7670_FPS [CODE]
        IMPORT OV7670_BriConLum [CODE]
        IMPORT OV7670_RGBgain [CODE]
        IMPORT OV7670_AWBmode [CODE]
        IMPORT __aeabi_i2f [CODE]
        IMPORT __aeabi_fdiv [CODE]
        IMPORT free_mem [CODE]
        IMPORT strlen [CODE]
        IMPORT mem_comp [CODE]
        IMPORT OV7670fps [DATA]
        IMPORT OV7670contrast [DATA]
        IMPORT OV7670luminis [DATA]
        IMPORT OV7670bright [DATA]
        IMPORT OV7670R_gain [DATA]
        IMPORT OV7670B_gain [DATA]
        IMPORT OV7670G_gain [DATA]
        IMPORT umbral [DATA]
        IMPORT modo [DATA]
        IMPORT umbral_x [DATA]
        IMPORT umbral_y [DATA]
        IMPORT http_config [DATA]
        IMPORT _printf_pre_padding [CODE]
        IMPORT _printf_truncate_unsigned [CODE]
        IMPORT _printf_truncate_signed [CODE]
        IMPORT _printf_sizespec [CODE]
        IMPORT _printf_percent [CODE]
        IMPORT _printf_return_value [CODE]
        IMPORT _printf_flags [CODE]
        IMPORT _printf_widthprec [CODE]
        IMPORT _printf_d [CODE]
        IMPORT _printf_i [CODE]
        IMPORT _printf_u [CODE]
        IMPORT _printf_o [CODE]
        IMPORT _printf_x [CODE]
        IMPORT _printf_c [CODE]
        IMPORT _printf_lc [CODE]
        IMPORT _printf_int_dec [CODE]
        IMPORT _printf_longlong_oct [CODE]
        IMPORT _printf_longlong_hex [CODE]
        IMPORT _printf_str [CODE]
        IMPORT _printf_wctomb [CODE]
        IMPORT __2sprintf [CODE]
        IMPORT __aeabi_fmul [CODE]
        IMPORT __aeabi_f2uiz [CODE]
        IMPORT det_x [DATA]
        IMPORT det_y [DATA]

        KEEP adv
        KEEP ||P2||

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
