#ifndef __NGV_BIOS_H_
#define __NGV_BIOS_H_


void ngv_setup();
void ngv_loop();

#ifdef __cplusplus
extern "C" {
#endif

void delay(int t);
void processEvent();

extern void _lcd_ctl_blk(unsigned char state);
extern void _lcd_ctl_cs(unsigned char state);
extern void _lcd_ctl_rs(unsigned char state);
extern void _lcd_ctl_rst(unsigned char state);

#ifdef __cplusplus
}
#endif


#endif
