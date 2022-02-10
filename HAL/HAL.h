/*
 * HAL.h
 *
 * Created: 12/14/2019 6:51:48 PM
 *  Author: Hesham Salem
 */ 


#ifndef HAL_H_
#define HAL_H_
#include "MCAL.h"
//=======================================7seg prototypes============================
void _7SegInit();
void _7SegDisplay();
//=======================================7seg definitions===========================
#define EN1  PB2
#define EN2  PB1
//=======================================LCD=========================================
#define EN PB0
#define RW PB1
#define RS PB2
#define CLEAR Lcdwritecmd(0x01)
#define SHIFTL Lcdwritecmd(0x18)
#define SHIFTR Lcdwritecmd(0x1C)
#define NEWLINE_1ST_ROW  Lcdwritecmd(0x80)
#define NEWLINE_2ND_ROW  Lcdwritecmd(0xC0)
#define NEWLINE_3RD_ROW  Lcdwritecmd(0x90)
#define NEWLINE_4TH_ROW  Lcdwritecmd(0xD0)
 
//=======================================LCD prototypes===============================
void Lcdwritedata(unsigned char data);
void Lcdwritecmd(unsigned char cmd);
void Genpulse();
void LcdInit();
void Lcdprintf(unsigned char* str);
//=====================================keypad prototypes =================
unsigned char Get_Key();
void KeyPad_Init();



#endif /* HAL_H_ */