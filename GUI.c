#include "ST7735.h"
#include "PLL.h"
#include "tm4c123gh6pm.h"
#include "IO.h"
#include "Print.h"
#include "GUI.h"
#include "random.h"
#define cyan	0x00ffff

int xval;
extern uint32_t ADCStatus;
extern uint32_t ADCMail;
extern int p2dir;
int oldPos1, pos2 = 50, oldPos2;
unsigned long color, difference;
void UpdateBall();

PType p1 = {50, 20, 0};

void DrawPlayer1(unsigned long pos, unsigned long color){
		p1.xPos = (pos * 0.98);
		ST7735_FillRect(p1.xPos, 10, 30, 10, color);
		ST7735_FillRect(0, 10, p1.xPos, 10, 0);
		ST7735_FillRect((p1.xPos + 30), 10, (128-(p1.xPos+30)), 10, 0);
}

PType p2 = {50, 140, 0};

void DrawPlayer2(unsigned long pos, unsigned long color){
		p2.xPos = (pos * 0.98);
		ST7735_FillRect(p2.xPos, 140, 30, 10, color);
		ST7735_FillRect(0, 140, p2.xPos, 10, 0);
		ST7735_FillRect((p2.xPos + 30), 140, (128-(p2.xPos+30)), 10, 0);
}

void DrawPlayers(unsigned long p1, unsigned long p2){
//	ST7735_FillRect(0, 10, 128, 10, 0);
	DrawPlayer1(p1, 0x0CFF);
	DrawPlayer2(p2, 0xFDA0);
}

void UpdatePlayer1(){
	uint32_t pos1 = ADCMail;		//read ADCMail (input)
	ADCStatus = 0;							//clear flag
	
	pos1 = Convert(pos1);				//convert the input to cm
	
	if(pos1 != oldPos1){
		DrawPlayer1(pos1, 0x0CFF);
		oldPos1 = pos1;
	}
}

void UpdatePlayer2(){
	if((Buttons_In() == 0x01)&&(pos2+30 <= 127)&&(pos2 >0)){
		pos2 += 1;
	}
	else if((Buttons_In() == 0x02)&&(pos2+30 <= 128)&&(pos2>1)){
		pos2 -= 1;
	}
	if(oldPos2 != pos2){
		DrawPlayer2(pos2, 0xFDA0);
		oldPos2 = pos2;
	}
}

PType ball = {64, 80, 0};

void DrawBall(uint16_t color){
	unsigned long posX, posY;
	posX = ball.xPos - 5;
	posY = ball.yPos - 5;
	ST7735_DrawCircle(posX,posY,color);
	
}


unsigned char ballCollisionCheck(){
	
	if( ball.yPos-5 == p1.yPos ){
		if( ( (p1.xPos-5) < ball.xPos ) && ((p1.xPos+35) > ball.xPos )  ){
			return(1);	
		}
		else{
			return(0);
		}
	}
	if( ball.yPos+5 == p2.yPos ){
		if( ( (p2.xPos-5) < ball.xPos ) && ((p2.xPos+35) > ball.xPos )  ){
			return(1);
		}
		else{
			return(0);
		}
	}else{
		return(0);
	}
	if((ball.yPos <= 5)){
		return(2);
	}
	if(ball.yPos >= 155){
		return(3);
	}
}

unsigned char ballWallCheck(){
	
	if(ball.xPos < 11){
		return(1);
	} 
	else if(ball.xPos > 107){
		return(1);
	}
	else if( ( (ball.xPos + 5) == p2.xPos) && ((ball.yPos+5) >=		p2.yPos)){
		return(1);
	}
	else if( ( ball.xPos == (p2.xPos+35))  && ((ball.yPos+5)  >=		p2.yPos)){
		return(1);
	}
	else if( ( (ball.xPos + 5) == p1.xPos) && ((ball.yPos-5) <=		p1.yPos+10)){
		return(1);
	}
	else if( ( ball.xPos == (p1.xPos+35))  && ((ball.yPos-5)  <=		p1.yPos)){
		return(1);
	}
	else{
		return(0);
	}
	
}

unsigned long speedY = 1;
unsigned long speedX = 1;
unsigned long difficulty = 25;
unsigned long speedCount = 25;
int stopped = 0;

void startGame( unsigned long spY, unsigned long spX, unsigned long dir){
	ST7735_FillScreen(0);
	ball.xPos = 64;
	ball.yPos = 80;
	speedY = spY;
	speedX = (Random5());
	if(speedX == 0){
		speedX += 1;
	}
	stopped = 0;
	speedCount = difficulty;
	DrawPlayers(p1.xPos, p2.xPos);
	DrawBall(0xF81D);
}

void displayScore(){
	//ST7735_SetCursor(29,40);
	if(p1.points >= 7){
		ST7735_SetCursor(4,7);
		ST7735_OutString("Player 1 Wins!");
	}
	else if(p2.points >= 7){
		ST7735_SetCursor(4,7);
		ST7735_OutString("Player 2 Wins!");
	}
	else{
	
		ST7735_SetCursor(10,4);
		ST7735_OutChar(p1.points + 0x30);
		ST7735_SetCursor(8,7);
		ST7735_OutString("Score");
		ST7735_SetCursor(10,10);
		ST7735_OutChar(p2.points + 0x30);
		ST7735_FillRect(29,40,5,90,0);
	}
		IO_Touch();
}


void stopGame(){
	speedY = 0;
	speedX = 0;
	ball.xPos = 64;
	ball.yPos = 80;
	DrawPlayers(p1.xPos,p2.xPos);
	displayScore();
	stopped = 0;
	startGame(1, 0, 0);
}

int a = 1;
extern unsigned long Timer2Count;
void playSound(unsigned char sound){
	Timer2A_Stop();
	Timer2_Init(40000000);
	if(a == 1){
		Timer2A_Start();
		Sound_Play(5500);
		a = 2;
	}
	else if(a == 2){
		Timer2A_Start();
		Sound_Play(6000);
		a = 1;
	}
}


void UpdateBall(){
	if(stopped == 0){
		if((ball.yPos <= 10) || (ball.yPos > 150)){
			if(ball.yPos <= 10){
				p2.points += 1;
			}
			if(ball.yPos > 150){
				p1.points += 1;
			}
			stopGame();
			stopped = -1;
		}
		else if(speedY != 0){
			
			if(speedCount <= 0){
				if(ballCollisionCheck() == 1){
					speedY = -speedY;
					playSound(1);
					if(difficulty > 5){
						difficulty = difficulty - 1;
					}
				}
				if(ballWallCheck() == 1){
					speedX = -speedX;
				}
					
					DrawBall(0);
					ball.yPos += speedY;
					ball.xPos += speedX;
					DrawBall(0xF81D);
					speedCount = difficulty;
			}
			speedCount--;
		}
	}
}