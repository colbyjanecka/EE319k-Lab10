// Lab7Main.c
// Runs on LM4F120/TM4C123
// Test the functions in ST7735.c by printing basic
// patterns to the LCD.
//    16-bit color, 128 wide by 160 high LCD
// Daniel Valvano
// Last Modified: 3/20/2018
// Ramesh Yerraballi modified 3/20/2017

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2017

 Copyright 2018 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// hardware connections
// **********ST7735 TFT and SDC*******************
// ST7735
// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"
#include "PLL.h"
#include "tm4c123gh6pm.h"
#include "IO.h"
#include "Print.h"
#include "images.h"
#include "Sound.h"
#include "DAC.h"
#include "GUI.h"

#define SIZE 16

uint32_t ADCMail;
uint32_t ADCStatus;

extern PType ball;
extern int stopped;
extern unsigned long Timer2Count;
extern unsigned long difficulty;
int globalCount;

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

 //#define F80HZ (50000000/80)
#define Cn  4777
#define Dn  4256
//#define C  14331
#define En  3791
#define Gn  3187


int main(void){  
  uint32_t i;
  PLL_Init(Bus80MHz);    // set system clock to 80 MHz
  IO_Init();
	Buttons_Init();
	Sound_Init();
	DAC_Init();
	ADC_Init();         // turn on ADC, set channel to 5
	Sound_Play(Dn);
  EnableInterrupts();
  ST7735_InitR(INITR_REDTAB);
	/**	
	for(i=4000; i<5000; i+100){
		Timer2A_Stop();
		Timer2_Init(40000000);
		Timer2A_Start();

		while(Timer2Count == 0){
			Sound_Play(i);
		}
		ST7735_FillScreen(50);
		ST7735_SetCursor(5,5);
		ST7735_OutChar(Timer2Count+ 0x30);
		IO_Touch();
	}
	**/
	ST7735_SetCursor(8,7);
	ST7735_OutString("Pong");
	
	IO_Touch();
	
	ST7735_FillScreen(0);
	
	Timer2_Init(40000000);
	Sound_Play(5500);
	Timer2A_Start();
	globalCount = 40000;

	while(globalCount != 0){
		globalCount--;
	}
	Timer2A_Stop();
	Sound_Play(6000);
	Timer2A_Start();
	globalCount = 40000;

	while(globalCount != 0){
		globalCount--;
	}
	Timer2A_Stop();
	Sound_Play(6500);
	Timer2A_Start();	
	globalCount = 40000;

	while(globalCount != 0){
		globalCount--;
	}
	
	
	Timer2_Init(40000000);	//Create 1 second timer
	Timer2A_Start();
	
	Timer1_Init(80000);	//Create 1 second timer
	Timer1A_Start();

	Timer0_Init(4000000/3); // /3
	Timer0A_Start();
	
	
	while(1){
		if(stopped == -1){
			IO_Touch();
			startGame(1,0,0);
		}
		
//		ST7735_FillScreen(0xFFFF);   // set screen to white
//		ST7735_DrawBitmap(44, 159, Logo, 40, 160);
//		IO_Touch();
//		ST7735_FillScreen(0);       // set screen to black
		 
	}
}

