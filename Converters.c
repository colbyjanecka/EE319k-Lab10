#include <stdint.h>
#include "tm4c123gh6pm.h"

uint32_t Convert2Dist(uint32_t input){
  uint32_t pos = 0;
	pos = ((input*(1825))/4505.5); // +133.68
	return (pos-0.002);
}

uint32_t Convert(uint32_t input){
  uint32_t pos = 0;
	pos = ((input*(100))/4105);
	return (pos);
}

uint32_t Random5(void){
  return ((Random32()>>24)%5)+1;  // returns 1, 2, 3, 4, or 5
}
