#include <stdio.h>
#include <RTL.h>
extern BOOL LEDrun;
extern BOOL LCDupdate;
extern BOOL tick;
extern U32  dhcp_tout;

#define MY_IP localm[NETIF_ETH].IpAdr
#define DHCP_TOUT   50                        /* DHCP timeout 5 seconds      */
void init_TCP (void);
void check_webserver(void);
void timer_poll (void) ;
void init_io (void);
int fputc(int ch, FILE *f);
void upd_display (void);
void init_display (void) ;
void dhcp_check (void);
void LED_out (U32 val);
void blink_led (void);
