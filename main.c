#include<at89x51.h>
#include"lcd.h"
#include"delay.h"

/*Defines*/
#define ok P2_0
#define up P2_1
#define buzzer P2_3
#define delay_time 50

/*Gloabal Variables*/
char c_hour=0x12,c_min=0x00,sec=0x00,clock_am_pm=0,timer=0;/*AM=0,PM=1*/
char a_hour=0x12,a_min=0x00,alarm_am_pm=0,op=0x0;

/*Function declaration*/
void update_time(void);/*Update Time after each second*/
void display_clock(void);/*Display Current time on LCD*/
void set_time(void);/*Set current time*/
void set_alarm(void);/*Set Alarm*/
char key_pressed(void);/*Detect Key pressed*/
void display_alarm(void);/*Display Alarm time on LCD*/

/*ISR for timer0*/
void timer0_isr(void) interrupt 1
{
	timer++;
	if(timer==14)
	{
		timer=0;
		update_time();
	}

}
void main(void)
{
	/*Timer initialisation*/
	TMOD = (TMOD & 0xF0) | 0x01;  /* Set T/C0 Mode */
	ET0 = 1;                      /* Enable Timer 0 Interrupts */
	TR0 = 0;                      /* Timer 0 off */
	EA = 1;                       /* Global Interrupt Enable */

	/*Buzzer initialisation*/
	buzzer=0;//output declared

	/*LCD Initialisation*/
	LCD_Init();
	LCD_GoToLineOne();
	LCD_Clear();

	/*Clock initialisation*/
	set_time();
	TR0 = 1;//Timer 0 on
	set_alarm();

	/*Program body*/
	while(1)
	{
		LCD_GoToLineOne();
		LCD_DisplayString("Alarm-> ");
		display_alarm();
		LCD_GoToLineTwo();
		display_clock();
		if(key_pressed()=='o')
			set_alarm();
		if(c_hour==a_hour && c_min==a_min && clock_am_pm==alarm_am_pm)//If Alarm time=Clock Time
		{
			buzzer=1;
		}
		else
		{
			buzzer=0;
		}
	}
}
void update_time(void)
{
	sec++;
	if((sec & 0x0F) > 0x09)//BCD correction
	{
		sec+=0x06;
	}
	if(sec>0x59)//sec counter reset
	{
		sec=0;
		c_min++;
	}
	if((c_min & 0x0F) > 0x09)//BCD correction
	{
		c_min+=0x06;
	}
	if(c_min>0x59)//c_min counter reset
	{
		c_min=0;
		c_hour++;
	}
	if((c_hour & 0x0F) > 0x09)//BCD correction
	{
		c_hour+=0x06;
	}
	if(c_hour>0x12)//c_hour counter reset and AM-PM correction
	{
		c_hour=0x01;
		clock_am_pm=!clock_am_pm;
	}
}

void display_clock(void)//displays Clock Time
{
		LCD_DisplayRtcTime(c_hour,c_min,sec);
		if(clock_am_pm)
			LCD_DisplayString(" PM");
		else
			LCD_DisplayString(" AM");
}
void display_alarm(void)//displays alarm time
{
		LCD_DataWrite(((a_hour>>4) & 0x0f) + 0x30);
    LCD_DataWrite((a_hour & 0x0f) + 0x30);
    LCD_DataWrite(':');

    LCD_DataWrite(((a_min>>4) & 0x0f) + 0x30);
		LCD_DataWrite((a_min & 0x0f) + 0x30);
		if(alarm_am_pm)
			LCD_DisplayString(" PM");
		else
		LCD_DisplayString(" AM");
}
char key_pressed(void)//detects key pressed
{
	if(ok==0)//pressed
	{
		delay_ms(delay_time);
		if(ok==0)//debounce checked
		{
			while(ok==0)
				ok=1;
			return 'o';
		}
	}
	if(up==0)//pressed
	{
		delay_ms(delay_time);
		if(up==0)////debounce checked
		{
			while(up==0)
				up=1;
			return 'u';
		}
	}
	else
		return 0x0;
}

void set_time(void)//set clock tiime
{
	LCD_Clear();
	LCD_GoToLineOne();
	LCD_DisplayString("Set Clock Time");
	//for setting hours
	do
	{
		op=0x0;
		LCD_GoToLineTwo();
		display_clock();
		while(op==0x0)
		{
			op=key_pressed();
		}
		if(op=='u')//If up key is pressed
		{
			if(c_hour!=0x12)
				c_hour++;
			else
				c_hour=0x01;
			if((c_hour & 0x0F) > 0x09)//hour correction for BCD
			{
				c_hour+=0x06;
			}
		}
	}while(op!='o');

	//for setting minutes
do
	{
		op=0x0;
		LCD_GoToLineTwo();
		display_clock();
		while(op==0x0)
		{
			op=key_pressed();
		}
		if(op=='u')//If up key is pressed
		{
			if(c_min!=0x59)
				c_min++;
			else
				c_min=0x00;
			if((c_min & 0x0F) > 0x09)//hour correction for BCD
			{
				c_min+=0x06;
			}
		}
	}while(op!='o');
	// For setting AM-PM
	do
	{
		op=0x0;
		LCD_GoToLineTwo();
		display_clock();
		while(op==0x0)
		{
			op=key_pressed();
		}
		if(op!='o')
			clock_am_pm=!clock_am_pm;
	}while(op!='o');
}

void set_alarm(void)
{
	LCD_Clear();
	LCD_GoToLineOne();
	LCD_DisplayString("Set Alarm Time");
	//for setting hours
	do
	{
		op=0x0;
		LCD_GoToLineTwo();
		display_alarm();
		while(op==0x0)
		{
			op=key_pressed();
		}
		if(op=='u')//If up key is pressed
		{
			if(a_hour!=0x12)
				a_hour++;
			else
				a_hour=0x01;
			if((a_hour & 0x0F) > 0x09)//hour correction for BCD
			{
				a_hour+=0x06;
			}
		}
	}while(op!='o');

	//for setting minutes
	do
	{
		op=0x0;
		LCD_GoToLineTwo();
		display_alarm();
		while(op==0x0)
		{
			op=key_pressed();
		}
		if(op=='u')//If up key is pressed
		{
			if(a_min==0x59)
				a_min=0x00;
			else
				a_min++;
			if((a_min & 0x0F) > 0x09)//min correction for BCD
			{
				a_min+=0x06;
			}
		}
	}while(op!='o');
	// For setting AM/PM
	do
	{
		op=0x0;
		LCD_GoToLineTwo();
		display_alarm();
		while(op==0x0)
		{
			op=key_pressed();
		}
		if(op!='o')
			alarm_am_pm=!alarm_am_pm;
	}while(op!='o');
}
