#include <stdint.h>
#include "tm4c123gh6pm.h"

unsigned long Timer1Count;
unsigned long Player2Count = 0;
unsigned long TimerLoopCounter = 0;
extern uint32_t ADCStatus;
extern uint32_t ADCMail;
extern uint32_t ADCMailOld;

void Timer1_Init(unsigned long period){
		unsigned long volatile delay;
		SYSCTL_RCGCTIMER_R |= 0x02; // 0) activate timer2
		delay = SYSCTL_RCGCTIMER_R;
		Timer1Count = 0;
		TIMER1_CTL_R = 0x00000000;		// 1) disable timer2A
		TIMER1_CFG_R = 0x00000000; 		// 2) 32-bit mode
		TIMER1_TAMR_R = 0x00000002; 	// 3) periodic mode
		TIMER1_TAILR_R = period-1; 		// 4) reload value
		TIMER1_TAPR_R = 0;				 		// 5) clock resolution
		TIMER1_ICR_R = 0x00000001; 		// 6) clear timeout flag
		TIMER1_IMR_R = 0x00000001; 		// 7) arm timeout
		NVIC_PRI5_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000;
																	// 8) priority 4
		NVIC_EN0_R = 1<<21; 					// 9) enable IRQ 23 in
		TIMER1_CTL_R = 0x00000001; 		// 10) enable timer2A
}
// trigger is Timer2A Time-Out Interrupt
// set periodically TATORIS set on rollover

void Timer1A_Handler(void){
		TIMER1_ICR_R = 0x00000001; // acknowledge
		Timer1Count++;
		TimerLoopCounter++;
		Player2Count++;
		// run some background stuff here
		UpdateBall();

		if(TimerLoopCounter = 30){
			UpdatePlayer1();
			if(Player2Count == 8){
				UpdatePlayer2();
				Player2Count = 0;
			}
			TimerLoopCounter = 0;
		}

}

void Timer1A_Stop(void){
		TIMER2_CTL_R &= ~0x00000001; // disable
}
void Timer1A_Start(void){
		TIMER2_CTL_R |= 0x00000001; // enable
}

