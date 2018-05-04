// Timer0.c
// Runs on LM4F120/TM4C123
// Use TIMER0 in 32-bit periodic mode to request interrupts at a periodic rate
// Daniel Valvano
// Last Modified: 3/6/2015 
// You can use this timer only if you learn how it works

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
  Program 7.5, example 7.6

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
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
#include <stdint.h>
#include "tm4c123gh6pm.h"

extern uint32_t ADCMail;
extern uint32_t ADCStatus;
extern uint32_t Data;        // 12-bit ADC
extern uint32_t Position;    // 32-bit fixed-point 0.001 cm
uint32_t ADCMailOld;

#define N 	  300
	
unsigned long Timer0Count;

void Timer0_Init(unsigned long period){
		unsigned long volatile delay;
		SYSCTL_RCGCTIMER_R |= 0x01; // 0) activate timer2
		delay = SYSCTL_RCGCTIMER_R;
		Timer0Count = 0;
		TIMER0_CTL_R = 0x00000000;		// 1) disable timer2A
		TIMER0_CFG_R = 0x00000000; 		// 2) 32-bit mode
		TIMER0_TAMR_R = 0x00000002; 	// 3) periodic mode
		TIMER0_TAILR_R = period-1; 		// 4) reload value
		TIMER0_TAPR_R = 0;				 		// 5) clock resolution
		TIMER0_ICR_R = 0x00000001; 		// 6) clear timeout flag
		TIMER0_IMR_R = 0x00000001; 		// 7) arm timeout
		NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000;
																	// 8) priority 4
		NVIC_EN0_R = 1<<19; 					// 9) enable IRQ 23 in
		TIMER0_CTL_R = 0x00000001; 		// 10) enable timer2A
}
// trigger is Timer2A Time-Out Interrupt
// set periodically TATORIS set on rollover

void Timer0A_Handler(void){
		TIMER0_ICR_R = 0x00000001; // acknowledge
		Timer0Count++;
		// run some background stuff here
		
		for(int i = 0; i < N; i++){
			ADCMail += ADC_In();
		}
		ADCMail = (ADCMail/N);
		//Set Flag
	
		ADCStatus = 1;
}

void Timer0A_Stop(void){
		TIMER0_CTL_R &= ~0x00000001; // disable
}
void Timer0A_Start(void){
		TIMER0_CTL_R |= 0x00000001; // enable
}





