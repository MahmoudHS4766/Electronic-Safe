/*
 * safe_project.c
 *
 * Created: 9/17/2023 12:51:07 AM
 *  Author: Mahmoud Hamdy
 */ 
#define F_CPU 8000000ul
#include <avr/delay.h>
#include "LCD.h"
#include "EEPROM_driver.h"
#include "Keybad_driver.h"
#define EEPROM_PASS_LOCATION 0x20
#define EEPROM_PASS_STATUS_LOCATION 0x0040

int main(void)
{
	char flag=0,i;
	char value = NOT_PRESSED , trials=2;
	char arr[4];
	LCD_Init();
	Keybad_Init();
		
	if(EEPROM_read(EEPROM_PASS_STATUS_LOCATION)==NOT_PRESSED)//0xFF
	{
		LCD_send_string("Set pass:");
		for (i=0 ; i<=3 ; i++)
		{
			do
			{
				value = Keybad_getKey();
			} while (value == NOT_PRESSED);
			
			LCD_sendChar(value);
			_delay_ms(500);
			LCD_move_cursor(1,10+i);
			LCD_sendChar('*');
			EEPROM_write(EEPROM_PASS_LOCATION+i,value);
		}
		EEPROM_write(EEPROM_PASS_STATUS_LOCATION,0x00);		
	}  
	
	while (flag==0)
	{
		arr[0]=arr[1]=arr[2]=arr[3]=NOT_PRESSED;
		LCD_clear_screen();
		LCD_send_string("Enter pass:");
		for(i=0 ; i<=3 ; i++)
		{
			do
			{
				arr[i] = Keybad_getKey();
			} while (arr[i]==NOT_PRESSED);
			LCD_sendChar(arr[i]);
			_delay_ms(500);
			LCD_move_cursor(1,12+i);
			LCD_sendChar('*');								
		}
		if (EEPROM_read(EEPROM_PASS_LOCATION)==arr[0] && (EEPROM_read(EEPROM_PASS_LOCATION+1))==arr[1] && (EEPROM_read(EEPROM_PASS_LOCATION+2))==arr[2] && (EEPROM_read(EEPROM_PASS_LOCATION+3))==arr[3])
		{
			LCD_clear_screen();
			LCD_send_string("Correct password");
			LCD_move_cursor(2,1);
			LCD_send_string(" \"safe is open\" ");
			flag=1;
		}
		else
		{

			LCD_clear_screen();
			LCD_send_string(" Wrong password");
			trials--;
				LCD_move_cursor(2,1);
				LCD_send_string("Trials left: ");
				LCD_move_cursor(2,14);
				LCD_sendChar(trials+48);
			_delay_ms(1500);
			if (trials==0)
			{
				LCD_clear_screen();
				LCD_send_string(" Safe is locked");
				LCD_move_cursor(2,1);
				LCD_send_string("Time left: ");
				for (i=15 ; i>0 ; i--)
				{
					LCD_move_cursor(2,12);
					if (i<10)
					{
						LCD_move_cursor(2,12);
						LCD_sendChar(' ');
						LCD_sendChar(i+48);
					} 
					else if(i<100)
					{
						LCD_sendChar(i/10+48);
						LCD_sendChar(i%10+48);
					}	
					_delay_ms(1000);
				}
				trials = 2;
			}
		}
	}		
}