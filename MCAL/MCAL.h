/*
 * MCAL.h
 *
 * Created: 12/14/2019 6:51:24 PM
 *  Author: Hesham Salem
 */ 


#ifndef MCAL_H_
#define MCAL_H_

#include "avr/io.h"
#include <stdint.h>
#include <string.h>
#include "avr/interrupt.h"
#define F_CPU  8000000
#include "util/delay.h" 
#define SETBIT(REG,BITNO)  REG|=(1<<BITNO)
#define CLRBIT(REG,BITNO)  REG&=~(1<<BITNO)
#define TOGGLEbIT(REG,BITNO)  REG^=(1<<BITNO)

unsigned char readbit (unsigned char REG, unsigned char BIT);
unsigned char* float_to_ascii(float num);
//=================================================interrupt==============================================
void interruptInit();
//================================================ADC=====================================================
void ADC_INIT();
unsigned short ADC_Read();
unsigned char* Read_analoge();
//================================================timerprototypes=========================================
void Timer_1_Init(void);
void GenerateSignal();
float C_angle(float angel);
//==============================================SPIprototypes============================================
void SPI_Master_init();
unsigned char SPI_M_sendchar(unsigned char data_trans);
void SPI_M_sendstring(unsigned char* str);
unsigned char read_SPDR();







#endif /* MCAL_H_ */