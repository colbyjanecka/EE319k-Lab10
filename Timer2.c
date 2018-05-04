#include <stdint.h>
#include "tm4c123gh6pm.h"

unsigned long Timer2Count;
extern int globalCount;

void Timer2_Init(unsigned long period){
		unsigned long volatile delay;
		SYSCTL_RCGCTIMER_R |= 0x04; // 0) activate timer2
		delay = SYSCTL_RCGCTIMER_R;
		Timer2Count = 0;
		TIMER2_CTL_R = 0x00000000;		// 1) disable timer2A
		TIMER2_CFG_R = 0x00000000; 		// 2) 32-bit mode
		TIMER2_TAMR_R = 0x00000002; 	// 3) periodic mode
		TIMER2_TAILR_R = period-1; 		// 4) reload value
		TIMER2_TAPR_R = 0;				 		// 5) clock resolution
		TIMER2_ICR_R = 0x00000001; 		// 6) clear timeout flag
		TIMER2_IMR_R = 0x00000001; 		// 7) arm timeout
		NVIC_PRI5_R = (NVIC_PRI3_R&0x00FFFFFF)|0x80000000;
																	// 8) priority 4
		NVIC_EN0_R = 1<<23; 					// 9) enable IRQ 23 in
		TIMER2_CTL_R = 0x00000001; 		// 10) enable timer2A
}
// trigger is Timer2A Time-Out Interrupt
// set periodically TATORIS set on rollover

void Timer2A_Handler(void){
		TIMER2_ICR_R = 0x00000001; // acknowledge
		// run some background stuff here
		int high;
		if(high == 1){
			IO_HeartBeat();
			high = 0;
		}
		else{
			high = 1;
		}
		//UpdatePlayer2();
		Sound_Play(0);
}

void Timer2A_Stop(void){
		TIMER2_CTL_R &= ~0x00000001; // disable
}
void Timer2A_Start(void){
		TIMER2_CTL_R |= 0x00000001; // enable
}

