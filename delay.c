/*----------------------------------------------------------------------------------
                      8051 delay library  with the crystal frequency 11.0592MHz
Filename: delay.c
Controller: P89V51RD2/89c51(8051 family)
Oscillator: 11.0592 MHz
Author: XploreLabz
website: www.xplorelabz.com
 ----------------------------------------------------------------------------------*/
#include<at89x51.h>


/*----------------------------------------------------------------------------------
                         void delay_us(unsigned int n)
 ----------------------------------------------------------------------------------
 * I/P Arguments: unsigned int.
 * Return value	: none

 * description :
         This function is used generate delay in us.
         It genarates a approximate delay of 10us for each count,
         if 5000 is passed as the argument then it generates a delay of apprx 50ms.

 note: 0(zero) should not be passed as the argument.
-----------------------------------------------------------------------------------*/
void delay_us(unsigned int us_count)
 {	 do
      {
		 us_count--;
	   }while(us_count);
   }



/*----------------------------------------------------------------------------------
                         void delay_ms(unsigned int n)
 -----------------------------------------------------------------------------------
 * I/P Arguments: unsigned int.
 * Return value	: none

 * description:
     This function is used generate delay in ms.
     It genarates a approximate delay of 1ms for each count,
     if 1000 is passed as the argument then it generates delay of apprx 1000ms(1sec)
-----------------------------------------------------------------------------------*/
void delay_ms(unsigned int ms_count)
 {
        while(ms_count!=0)
		 {
	        delay_us(112);	 //delay_us is called to generate 1ms delay
			 ms_count--;
			}

   }
/*Actual library consisted of many functions. Truncated to save flash space for AT89S51 microcontroller*/