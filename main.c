/*
 * embeddedlab1.c
 *
 * Created: 12/14/2019 6:48:00 PM
 * Author : Hesham Salem
 */ 

#include <HAL.h>
#define SAFE_PASSWORD 1


ISR(INT0_vect)
{
	CLEAR;
	Lcdprintf("FIRE ALARM");
	SETBIT(DDRD,PD4);
	SETBIT(PORTD,PD4);
	_delay_ms(5000);
	CLRBIT(PORTD,PD4);
	CLEAR;
	
	
}

ISR(ADC_vect)
{
	CLEAR;
	Lcdprintf(Read_analoge());
	SETBIT(ADCSRA,ADSC);		
}


ISR(INT1_vect)
{
	SETBIT(ADCSRA,ADSC);
	CLEAR;
	NEWLINE_1ST_ROW;
	Lcdprintf("BRIGHTNESS");
	unsigned short data = ADC;
	for (int i= 0;i<100; i++)
	{
		if (data != ADC)
		{
			data = ADC;
			i = 0;
		}
		NEWLINE_2ND_ROW;
		
		Lcdprintf(Read_analoge());
		
		
	}
	CLEAR;
	NEWLINE_1ST_ROW;
	Lcdprintf("Enter Password");
	NEWLINE_2ND_ROW;
}
int main(void)
{
    /* Replace with your application code */
	Timer_1_Init();
	interruptInit();
	ADC_INIT();
	//SETBIT(ADCSRA,ADSC);
	SPI_Master_init();
	
	//SPI_M_sendstring("FEDAA#");
	
	//_delay_ms(100);
	//Lcdwritedata(SPI_M_sendchar(' '));
	
	/*Timer_0_Init();
	
	
	unsigned char str[100+1]={0};
	sprintf(str,"%d%s",23,"fedaa");
	Lcdprintf(str);
	Lcdprintf(float_to_ascii(3.4));
	
	*/
	


	#if SAFE_PASSWORD
	//======================================================password=============================================================
	LcdInit();
	KeyPad_Init();
	unsigned char original_password[4+1] = "2196";
	unsigned char entered_password[4+1] = {'\0'};
	int j;
	
	for (int i=0;i<3;i++) //3 trials only
	{
		CLEAR;
		NEWLINE_1ST_ROW;
		Lcdprintf("Enter password");
		NEWLINE_2ND_ROW;
		for ( j=0;j<4;j++) // getting password
		{
			entered_password[j] = Get_Key();
			Lcdprintf("*");  // to hide the output
			SPI_M_sendchar(entered_password[j]);
		}
		//entered_password[j]='#';
		//SPI_M_sendstring(entered_password);
		_delay_ms(100);
		if (SPI_M_sendchar('#')=='1')
		{
			CLEAR;
			NEWLINE_1ST_ROW;
			Lcdprintf("Correct password");
			_delay_ms(2000);
			break;
			
			
		}
		else
		{
			NEWLINE_1ST_ROW;
			Lcdprintf("Wrong password");
			_delay_ms(2000);
		}
		if (i==2)
		{
					CLEAR;
					NEWLINE_1ST_ROW;
					Lcdprintf("3 times wrong");
					NEWLINE_2ND_ROW;
					Lcdprintf("safe is locked");
					while(1);
			
		}
		
	}
	
	CLEAR;
	Lcdprintf("Choose ");
	NEWLINE_2ND_ROW;
	Lcdprintf("1-Open safe");
	NEWLINE_3RD_ROW;
	Lcdprintf("2-Close safe");
	NEWLINE_4TH_ROW;
	Lcdprintf("3-Change Password");
	
	while(1)
	{
		
	switch(Get_Key())
	{
		case '1': OCR1A = C_angle(0);
		NEWLINE_1ST_ROW;
		Lcdprintf("safe opened");
		break;
		case '2': OCR1A = C_angle(90);
		NEWLINE_1ST_ROW;
		Lcdprintf("safe closed");
		break;
	}
	}
	#endif
	
    while (1)
    {
	 }

}
