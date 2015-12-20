#ifndef _LCD_H
#define _LCD_H

void LCD_Init();
void LCD_EnablePulse();
void LCD_CmdWrite(char);
void LCD_DataWrite(char);
void LCD_Clear();
void LCD_GoToLineOne();
void LCD_GoToLineTwo();
void LCD_DisplayString(char *);
void LCD_DisplayRtcTime(char, char, char);


#endif