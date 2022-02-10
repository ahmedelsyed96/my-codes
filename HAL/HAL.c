
/*
 * HAL.c
 *
 * Created: 12/14/2019 6:49:37 PM
 *  Author: Hesham Salem
 */ 

#include "HAL.h"
#define _7seg4pins 0
#define KEYPAD4  0
#define  LCD8BIT  0
#define LCD4BIT  0
#define KEYPAD3  0


//================================================7 segment intialtization ==================================================
 void _7SegInit()
{
	#if _7seg4pins
	
	DDRA|= 0xF0;
	SETBIT(DDRB,EN2);
	SETBIT(DDRB,EN1);
	
	
	
	
	#else
	/*
	for (int i=PC0; i<PC7 ; i++)
	{
		SETBIT(DDRPORT,i); 
	} */ 
	DDRC|= 0x7F;
	SETBIT(DDRD,EN2);
	SETBIT(DDRD,EN1);
	#endif
}

//==================================================7 segment display =======================================================
void _7SegDisplay()
{
	#if _7seg4pins
	
	uint8_t _7SegArray[]= {0x00,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80,0x90};
		for (int i=0 ;i<10;i++)
		{
			for (int j=0; j<10;j++)
			{
				for(int k=0;k<500;k++)
				{
					PORTA= _7SegArray[i];
					SETBIT(PORTB,EN1);
					_delay_ms(1);
					CLRBIT(PORTB,EN1);
					PORTA= _7SegArray[j];
					SETBIT(PORTB,EN2);
					_delay_ms(1);
					CLRBIT(PORTB,EN2);
				}
			}
			
		}
		
	#else
	//SETBIT(DDRC,PC7);
	uint8_t _7SegArray[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
		for (int i = 0; i<10;i++)
     	{
			for (int j=0; j<10;j++)
		      {
				  for(int k=0; k<500;k++)
				  {
					  PORTC= _7SegArray[j]^0x7f;
					  SETBIT(PORTD,EN1);
		              _delay_ms(1);
	                 CLRBIT(PORTD,EN1);
		             PORTC = _7SegArray[i]^0x7f;
				     SETBIT(PORTD,EN2);
		             _delay_ms(1);
		             CLRBIT(PORTD,EN2);
				  }
		      }
		
		
		
		/*
		if (i >=5)
		{
			SETBIT(PORTC,PC7);
		}
		else
		{
			CLRBIT(PORTC,PC7);
		} */
		
		}
		
		#endif
		
	   
}

//=========================================================LCD===============================================================


void LcdInit()
{
	#if LCD8BIT
	
	//set DIR of PORTA as output
	DDRA=0xff;
	SETBIT(DDRB,RS);
	SETBIT(DDRB,EN);
	SETBIT(DDRB,RW);
	//LCD is in write mode
	CLRBIT(PORTB,RW);
	
	Lcdwritecmd(0x02);  //back to home
	Lcdwritecmd(0x38);  //2 lines 8 bits
	Lcdwritecmd(0x01);  // clear LCD
	Lcdwritecmd(0x0C);  // show cursor
  #else
	//set DIR of PORTD as output
	for(int i=4;i<8;i++)
	{
		SETBIT(DDRA,i);
	}
	PORTA&=0x0F;
	SETBIT(DDRB,RS);
	SETBIT(DDRB,EN);
	SETBIT(DDRB,RW);
	//LCD is in write mode
	CLRBIT(PORTB,RW);
	Lcdwritecmd(0x02); //back to home
	Lcdwritecmd(0x28); //4bit mode
	Lcdwritecmd(0x01);  // clear LCD
	Lcdwritecmd(0x0C);  // show cursor
	NEWLINE_1ST_ROW;
	Lcdwritecmd(0x06);  // increment cursor right
	_delay_ms(20);
	

	#endif
}

// send command
void Lcdwritecmd(unsigned char cmd)
{
	#if LCD8BIT
	CLRBIT(PORTB,RS);
	PORTA=cmd;
	Genpulse();
	
	#else 
	CLRBIT(PORTB,RS);
	// clear the 4 MSB
	for (int i=4; i<8 ; i++)
	{
		CLRBIT(PORTA,i);
	}
	PORTA|=(cmd&0xf0);
	Genpulse();
	//clear 4 MSB
	for(int i=4;i<8;i++)
	{
		CLRBIT(PORTA,i);
	}
	//shifting the LSB to the  MSB
	PORTA|=((cmd&0x0f)<<4);
	Genpulse(); 
	#endif	
}

//send data
void Lcdwritedata(unsigned char data)
{
	#if LCD8BIT
	SETBIT(PORTB,RS);
	PORTA=data;
	Genpulse();
	#else 
	
	SETBIT(PORTB,RS);
	// clear the 4 MSB
	for (int i=4; i<8 ; i++)
	{
		CLRBIT(PORTA,i);
	}
	PORTA|=(data&0xf0);
	Genpulse();
	//clear 4 MSB
	for(int i=4;i<8;i++)
	{
		CLRBIT(PORTA,i);
	}
	//shifting the LSB to the  MSB
	PORTA|=((data&0x0f)<<4);
	Genpulse();
	#endif
}



//======================================pulse generation=============================================================
void Genpulse()
{
	SETBIT(PORTB,EN);
	_delay_ms(1);
	CLRBIT(PORTB,EN);
	_delay_ms(1);
}


//LCD write string

void Lcdprintf(unsigned char* str)
{
	int i;
 	for(i=0;i<strlen((const char*)str);i++)
	 {
		 Lcdwritedata(str[i]);
	 }
}


//====================================KEYPAD 4*4============================================

void KeyPad_Init()
{
	//make first 4 bits in REG-C As out put
	DDRC|=0xf0; // pc7,Pc6,Pc5 ,Pc4   1111 0000
	//enable pull up
	PORTC|=0x0f;
}

//read from keypad
unsigned char Get_Key()
{

	while(1)
	{
		PORTC|=0xf0;
		for(int i=PC4;i<=PC7;i++)
		{
			CLRBIT(PORTC,i);
			unsigned char state;
			state =(PORTC&0xf0);
		//	1011 0000=
		//	1111 0000
		//	1011 0000
		//	0x e 0
			switch (state)
			{
				case 0xe0:
				if(readbit(PINC,PINC0)==0)
				{
					while(readbit(PINC,PINC0)==0);
					return '7';
				}
				else if(readbit(PINC,PINC1)==0)
				{
					while(readbit(PINC,PINC1)==0);
					return '4';
				}
				else if(readbit(PINC,PINC2)==0)
				{
					while(readbit(PINC,PINC2)==0);
					return '1';

				}
				else if(readbit(PINC,PINC3)==0)
				{
					while(readbit(PINC,PINC3)==0);
					 CLEAR;

				}

				break;
				case 0xd0:

				if(readbit(PINC,PINC0)==0)
				{
					while(readbit(PINC,PINC0)==0);
					return '8';
				}
				else if(readbit(PINC,PINC1)==0)
				{
					while(readbit(PINC,PINC1)==0);
					return '5';
				}
				else if(readbit(PINC,PINC2)==0)
				{
					while(readbit(PINC,PINC2)==0);
					return '2';

				}
				else if(readbit(PINC,PINC3)==0)
				{
					while(readbit(PINC,PINC3)==0);
					return '0';

				}
				break;

				case 0xb0:
				// 1011 0000
				//0x b 0

				if(readbit(PINC,PINC0)==0)
				{
					while(readbit(PINC,PINC0)==0);
					return '9';
				}
				else if(readbit(PINC,PINC1)==0)
				{
					while(readbit(PINC,PINC1)==0);
					return '6';
				}
				else if(readbit(PINC,PINC2)==0)
				{
					while(readbit(PINC,PINC2)==0);
					return '3';

				}
				else if(readbit(PINC,PINC3)==0)
				{
					while(readbit(PINC,PINC3)==0);
					return '=';

				}

				break;
				case 0x70:
				// 0111 0000
				//0x 7 0

				if(readbit(PINC,PINC0)==0)
				{
					while(readbit(PINC,PINC0)==0);
					return '/';
				}
				else if(readbit(PINC,PINC1)==0)
				{
					while(readbit(PINC,PINC1)==0);
					return '*';
				}
				else if(readbit(PINC,PINC2)==0)
				{
					while(readbit(PINC,PINC2)==0);
					return '-';

				}
				else if(readbit(PINC,PINC3)==0)
				{
					while(readbit(PINC,PINC3)==0);
					return '+';

				}

				break;

			}
			SETBIT(PORTC,i);
		}
	}
	return 0;
}




