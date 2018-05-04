// IO.c
// This software configures the switch and LED
// You are allowed to use any switch and any LED, 
// although the Lab suggests the SW1 switch PF4 and Red LED PF1
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: March 30, 2018
// Last Modified:  change this or look silly
// Lab number: 7


#include "tm4c123gh6pm.h"
#include <stdint.h>

//------------IO_Init------------
// Initialize GPIO Port for a switch and an LED
// Input: none
// Output: none
int delay;
void IO_Init(void) {
 // --UUU-- Code to initialize PF4 and PF2
	SYSCTL_RCGCGPIO_R |= 0x33;
	delay = SYSCTL_RCGCGPIO_R;
	
	GPIO_PORTF_DIR_R &= ~0x10;
	GPIO_PORTF_DIR_R |= 0x04;
	
	GPIO_PORTF_AFSEL_R &= ~0x14;
	GPIO_PORTF_DEN_R |= 0x14;
	GPIO_PORTF_PUR_R |= 0x10;

}

void Buttons_Init(void){
		SYSCTL_RCGCGPIO_R |= 0x12;
		volatile unsigned long delay;
		delay = SYSCTL_RCGCGPIO_R;
	
		GPIO_PORTE_DIR_R &= ~0x0F;
		GPIO_PORTE_AFSEL_R &= ~0x0F;
		GPIO_PORTE_DEN_R	|= 0x0F;
	
}

uint32_t Buttons_In(void){
  uint32_t Input;
	Input = (GPIO_PORTE_DATA_R & 0x03);
  return Input;
}


//------------IO_HeartBeat------------
// Toggle the output state of the  LED.
// Input: none
// Output: none
void IO_HeartBeat(void) {
 // --UUU-- PF2 is heartbeat
	GPIO_PORTF_DATA_R ^= 0x04;
}


//------------IO_Touch------------
// wait for release and press of the switch
// Delay to debounce the switch
// Input: none
// Output: none
void IO_Touch(void) {
 // --UUU-- wait for release; delay for 20ms; and then wait for press
	while((GPIO_PORTF_DATA_R & 0x10) != 0){
	}
	while((GPIO_PORTF_DATA_R & 0x10) == 0){
	}
	int i = 20;
	while((i != 0) && ((GPIO_PORTF_DATA_R & 0x10) != 0)){
		int k = 6200;
		while(k != 0){
			k--;
		}
		i--;
	}
}  

