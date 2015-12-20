/*----------------------------------------------------------------------------------
                        8051 LCD library for 4-bit mode
Filename: lcd_4_bit.c
Controller: P89V51RD2/89c51(8051 family)
Oscillator: 11.0592 MHz
Author: XploreLabz
website: www.xplorelabz.com

 ----------------------------------------------------------------------------------
Note:
 1.Pin connection for LCD display in 4-bit mode.
 2.By default the LCD is connected to Port0.
 3.The code can be modified to connect the LCD to any of the PORTs by changing the
   "#define databus P0".
            _______________________________________________
           |                                               |
           |              16*2 lcd  Display                |
           |                4-bit Mode                     |
           |                                               |
           |                                               |
           |                                               |
           |         D7 D6 D5 D4 D3 D2 D1 D0     EN RW RS  |
            -----------------------------------------------
                     |  |  |  |                  |  |  |
                     |  |  |  |					 |	|  |
                     |	|  |  |             	 |  |  |___P0.2
					 |  |  |  | 	             |  |______P0.1
                     |  |  |  |                  |_________P0.0
					 |	|  |  |
					 |	|  |  |____________________________P0.4
					 |  |  |_______________________________P0.5
					 |  |__________________________________P0.6
					 |_____________________________________P0.7


----------------------------------------------------------------------------------*/


#include<at89x51.h>
#include "delay.h"
#include "lcd.h"

#define databus	P0       //	LCD databus connected to PORT1

sbit rs= databus^0;		 // Register select pin connected to P0.2
sbit rw= databus^1;		 // Read Write pin connected to P0.1
sbit en= databus^2;		 // Enable pin connected to P0.0



/* 16x2 LCD Specification */
#define LCDMaxLines 2
#define LCDMaxChars 16
#define LineOne 0x80
#define LineTwo 0xc0

#define BlankSpace ' '


/*----------------------------------------------------------------------------------
                     void LCD_EnablePulse()
 ----------------------------------------------------------------------------------
 * Function name: LCD_EnablePulse()
 * I/P Arguments: none.
 * Return value	: none
 * description  :This function is used to provide a enable pulse to latch data/command 
 LCD, since it is used frequently making it a function.
----------------------------------------------------------------------------------*/

void LCD_EnablePulse()
{
 /* refer timing diagram on page 26 HDD44780 for more
 It is observed that 0 to 1 and 1 to 0 transistions for 8051 
 are not sharp, hence enable pulse width is kept higher*/
 en = 0; //ensure low
 en=1;
 delay_us(100); // pulse width > 230 nsec
 en = 0;
 delay_us(10); // hold time >10nsec
}


/*----------------------------------------------------------------------------------
                         void LCD_Init()
 ----------------------------------------------------------------------------------
 * Function name:  LCD_Init()
 * I/P Arguments: none.
 * Return value	: none

 * description  :This function is used to initialize the lcd in 4-bit mode

----------------------------------------------------------------------------------*/
void LCD_Init()
{
    databus = 0x00; //output
    rs=0;
    rw=0;

   	databus = 0x30;
	LCD_EnablePulse();
	delay_ms(100);//recommendded >4.1 ms
	
	databus = 0x30;
	LCD_EnablePulse();
	delay_ms(1); //>100us
	
	databus = 0x30;
	LCD_EnablePulse();
	delay_ms(1); //>100us

	databus = 0x20;
    LCD_EnablePulse();
	delay_us(200); //>100us
	
	//reset sequence ends here
 	
   LCD_CmdWrite(0x28);	 //Initilize the LCD in 4bit Mode
   LCD_CmdWrite(0x0E);	// Display ON cursor ON
   LCD_CmdWrite(0x01);	// Clear the LCD
   LCD_CmdWrite(0x80);	// Move the Cursor to First line First Position

}




/*---------------------------------------------------------------------------------
                         void LCD_Clear()
 ----------------------------------------------------------------------------------
 * I/P Arguments: none.
 * Return value	: none

 * description  :This function clears the LCD and moves the cursor to first Position

-----------------------------------------------------------------------------------*/
void LCD_Clear()
{
   LCD_CmdWrite(0x01);	// Clear the LCD and go to First line First Position
}





/*---------------------------------------------------------------------------------
                         void LCD_GoToLineOne()
 ----------------------------------------------------------------------------------
 * I/P Arguments: none.
 * Return value	: none

 * description  :This function moves the Cursor to First line First Position

-----------------------------------------------------------------------------------*/
void LCD_GoToLineOne()
{
   LCD_CmdWrite(LineOne);	// Move the Cursor to First line First Position
}




/*---------------------------------------------------------------------------------
                         void LCD_GoToLineTwo()
 ----------------------------------------------------------------------------------
 * I/P Arguments: none.
 * Return value	: none

 * description  :This function moves the Cursor to Second line First Position

-----------------------------------------------------------------------------------*/
void LCD_GoToLineTwo()
{
   LCD_CmdWrite(LineTwo);	// Move the Cursor to Second line First Position
}



/*----------------------------------------------------------------------------------
                       void LCD_CmdWrite( char cmd)
------------------------------------------------------------------------------------
 * I/P Arguments: 8-bit command supported by LCD.
 * Return value	: none

 * description  :This function sends a command to LCD in the following steps.
     steP0: Send the Higher Nibble of the I/P command to LCD.
     step2: Select the Control Register by making RS low.
     step3: Select Write operation making RW low.
	 step4: Send a High-to-Low pulse on Enable PIN with some delay_us.

	 step5: Send the Lower Nibble of the I/P command to LCD.
	 step6: Select the Control Register by making RS low.
	 step7: Select Write operation making RW low.
	 step8: Send a High-to-Low pulse on Enable PIN with some delay_us.
----------------------------------------------------------------------------------*/
void LCD_CmdWrite( char cmd)
{

   databus=(cmd & 0xf0);        // Send the Higher Nibble of the command to LCD
     rs=0;						// Select the Command Register by pulling RS LOW
     rw=0;						// Select the Write Operation  by pulling RW LOW
   LCD_EnablePulse();

   delay_us(500);					// wait for some time

   databus=((cmd<<4) & 0xf0);   // Send the Lower Nibble of the command to LCD
     rs=0;						// Select the Command Register by pulling RS LOW
     rw=0;						// Select the Write Operation  by pulling RW LOW
    LCD_EnablePulse();

   delay_us(500);
}








/*---------------------------------------------------------------------------------
                       void LCD_DataWrite( char dat)
 ----------------------------------------------------------------------------------
 * Function name: LCD_DataWrite()
 * I/P Arguments: ASCII value of the char to be displayed.
 * Return value	: none

 * description  :
    This function sends a character to be displayed on LCD in the following steps.
       steP0: Send the higher nibble of the character to LCD.
       step2: Select the Data Register by making RS high.
       step3: Select Write operation making RW low.
	   step4: Send a High-to-Low pulse on Enable PIN with some delay_us.

       step5: wait for some time

       step6: Send the lower nibble of the character to LCD.
	   step7: Select the Data Register by making RS high.
	   step8: Select Write operation making RW low.
	   step9: Send a High-to-Low pulse on Enable PIN with some delay_us.
----------------------------------------------------------------------------------*/
void LCD_DataWrite( char dat)
{


   databus=(dat & 0xf0);	    // Send the Higher Nibble of the Data to LCD
     rs=1;						// Select the Data Register by pulling RS HIGH
     rw=0;						// Select the Write Operation  by pulling RW LOW
    LCD_EnablePulse();

   delay_us(500);			        // wait for some time.

   databus=((dat <<4) & 0xf0);	// Send the Lower Nibble of the Data to LCD
     rs=1;						// Select the Data Register by pulling RS HIGH
     rw=0;						// Select the Write Operation  by pulling RW LOW
    LCD_EnablePulse();
    delay_us(500);
}






/*---------------------------------------------------------------------------------
                       void LCD_DisplayString(char *string_ptr)
 ----------------------------------------------------------------------------------
 * Function name:  lcd_display_string()
 * I/P Arguments: String(Address of the string) to be displayed.
 * Return value	: none

 * description  :
               This function is used to display the ASCII string on the lcd.
                 1.The string_ptr points to the first char of the string
                    and traverses till the end(NULL CHAR).
                 2.Each time a char is sent to lcd_dat funtion to display.
-----------------------------------------------------------------------------------*/
void LCD_DisplayString(char *string_ptr)
{
   while(*string_ptr)
    LCD_DataWrite(*string_ptr++);
	}

/*---------------------------------------------------------------------------------
                  LCD_DisplayRtcTime(char hour,char min,char sec)
 ----------------------------------------------------------------------------------
 * I/P Arguments: char hour,char min,char sec
             hour,min,sec should be packed BCD format, as read from DS1307

 * Return value	: none

 * description  :This function display hour,min,sec read from DS1307.

   ___________ Display the higher nibble of hour after adding 0x30(ASCII conversion)
  |            Display the lower nibble of hour after adding 0x30(ASCII conversion)
  |
  |	  ________ Display the higher nibble of min after adding 0x30(ASCII conversion)
  |	 |     	   Display the lower nibble of min after adding 0x30(ASCII conversion)
  |	 |
  |	 |	 _____ Display the higher nibble of sec after adding 0x30(ASCII conversion)
  |	 |	|      Display the lower nibble of sec after adding 0x30(ASCII conversion)
  |	 |	|
  10;40;07

-----------------------------------------------------------------------------------*/
void LCD_DisplayRtcTime(char hour,char min,char sec)
{
    LCD_DataWrite(((hour>>4) & 0x0f) + 0x30);
    LCD_DataWrite((hour & 0x0f) + 0x30);
    LCD_DataWrite(':');

    LCD_DataWrite(((min>>4) & 0x0f) + 0x30);
	LCD_DataWrite((min & 0x0f) + 0x30);
    LCD_DataWrite(':');

    LCD_DataWrite(((sec>>4) & 0x0f) + 0x30);
	LCD_DataWrite((sec & 0x0f) + 0x30);

 }
/*Actual library consisted of many functions. Truncated to save flash space for AT89S51 microcontroller*/