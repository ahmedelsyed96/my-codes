
/*
 * MCAL.c
 *
 * Created: 12/14/2019 6:49:22 PM
 *  Author: Hesham Salem
 */ 

#include "MCAL.h"

unsigned char readbit (unsigned char REG, unsigned char BIT)
{

if (REG&(1<<BIT))
{
	return 1;
}
else
{
	return 0;
}
}

void interruptInit()
{
	CLRBIT(DDRD,PD3); 
	PORTD|= (1<<3); 
	//setting PD2 as input to be an external interrupt
	SETBIT(MCUCR,ISC00);
	SETBIT(MCUCR,ISC01);  
	//setting interrupt to be rising edge
	
	SETBIT(SREG,7); //Enabling the global interrupt 
	
	//SETBIT(GICR,INT0); 
	//Enabling interrupt 0
	SETBIT(GICR,INT1);
	//Enabling interrupt 1
	
}

void ADC_INIT()
{
	//make direction of DDA1 is input
	CLRBIT(DDRA,DDA1);
	//internal voltage 2.56 and with external cap at vref
	SETBIT(ADMUX,REFS0);
	//SETBIT(ADMUX,REFS1);
	//Enable ADC1 in PORTA
	SETBIT(ADMUX,MUX0);
	//Enable ADC
	SETBIT(ADCSRA,ADEN);
	//Enable Global interrupt flag
	//SETBIT(SREG,7);
	//Enable interrupt
	//SETBIT(ADCSRA,ADIE);
	//use prescaler 64 as ADC use freq from 50khz to 200 khz
	SETBIT(ADCSRA,ADPS1);
	SETBIT(ADCSRA,ADPS2);
	_delay_ms(10);

}


unsigned short ADC_Read()
{
	//Start Conversion
	SETBIT(ADCSRA,ADSC);
	//wait until conversion complete
	while(readbit(ADCSRA,ADIF)==0);
	return ADC;

}

unsigned char* Read_analoge()
{
	unsigned short digitaldata= ADC_Read();
	float  AnalogueData=0.0;
	AnalogueData=(float)(digitaldata*10)/1023;
	return (float_to_ascii(AnalogueData));

}

unsigned char* float_to_ascii(float num)
{
	static unsigned char str[10]={0};
	char strtemp[10]={0};
	int Intpart=(int)num;
	float Fltpart=num-Intpart;
	int DecimalCounter =0;
	float temp=Fltpart;
	
	while (temp<1.0 && temp!=0.0)
	{
		temp*=10;
		DecimalCounter++;
		
	}
	temp*=10;
	if (DecimalCounter!=0)
	{
		memset(strtemp,'0',DecimalCounter-1);
	}
	sprintf((char*)str,"%d.%s%d",Intpart,strtemp,(int)temp);
	return str;
}

void Timer_1_Init(void)
{
    SETBIT(DDRD,DDD5); //setting OC1 as PWM output
	TCCR1A|= (1<<COM1A1)|(1<<WGM11); //choosing PWM mode
	TCCR1B|=(1<<WGM12)|(1<<CS11)|(1<<WGM13); //prescaler = 8 so 1 step = 1us  -> 8/Fcpu = 1us
	ICR1 = 19999 ; // total period equal 20ms
	
}
void GenerateSignal()
{
	OCR0=(ADC_Read()>>2);
}

float C_angle(float angel)
{
	float value =(((angel-(-90))*(2-1))/(90-(-90)))+1;  //interpolation
	return (value *19999/20);
}

//=====================================SPI master==========================

void SPI_Master_init()
{
	DDRB |=(1<<PB4)|(1<<PB5)|(1<<PB7);
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR1); //enabling SPI protocol and setting this MCU as master
	
}

unsigned char SPI_M_sendchar(unsigned char data_trans)
{
    static unsigned char data_recieved;
	SPDR = data_trans;
	while(readbit(SPSR,SPIF)==0); // to wait until finish sending data
	data_recieved=SPDR;
	
	return data_recieved;
	
}


void SPI_M_sendstring(unsigned char* str)
{
	while(*str!='\0')
	{
		SPI_M_sendchar(*str);
		str++;
	}
}

unsigned char read_SPDR()
{
	while(readbit(SPSR,SPIF)==0);
	return SPDR;
}