#include <CoOS.h>			              /*!< CoOS header file	         */
#include <stdio.h>
#include "stm32f4xx_conf.h"
#include "LCD.h"
#include "BSP.h"
#include "LedTask.h"

#define STACK_SIZE_LCD 1024              /*!< Define "taskA" task size */
OS_STK     LCD_stk[6][STACK_SIZE_LCD];	  /*!< Define "taskA" task stack */


void LCDManagerTask (void* pdata);
void LCDBallControlTask(void * parg);
void LCDRacket0ControlTask(void * parg);
void LCDRacket1ControlTask(void * parg);
void LCDInitScreenControl(void * parg);
void gameOverTask(void * parg);
void InitScreen4Game(void);

uint8_t global_racket0_posY;
uint8_t global_racket1_posY;

uint8_t score0;
uint8_t score1;

uint16_t pos_init;

#define directionY_up 0
#define directionY_down 1

#define directionX_left 0
#define directionX_right 1

#define racket_width 8
#define racket_height 40

#define left_limit 15
#define right_limit 303
#define top_limit 7
#define low_limit 231

OS_EventID racket0_sem;
OS_EventID racket1_sem;

void CreateLCDTask(void){

	Init_AnalogJoy();

	CoCreateTask (LCDManagerTask,0,1,&LCD_stk[0][STACK_SIZE_LCD-1],STACK_SIZE_LCD);

}


void LCDManagerTask (void* pdata) {

	OS_TID lcdId, ballId, l_racketId, r_racketId, game_overId;
	int i = 0;

	CoTickDelay(1000);
    LCD_Initialization();

  for (;;) {

	  lcdId = CoCreateTask (LCDInitScreenControl, 0, 1, &LCD_stk[1][STACK_SIZE_LCD-1], STACK_SIZE_LCD);

	  waitForKey(0, 0);
	  CoDelTask(lcdId);
	  score0 = 0;
	  score1 = 0;

	  while (score0 < 3 && score1 < 3) {

		  if (i == 0) {
			  pos_init = 15;
			  i = 1;
		  } else {
			  pos_init = 299;
			  i = 0;
		  }

		  InitScreen4Game();

		  waitForKey(0, 0);

		  ballId = CoCreateTask (LCDBallControlTask, 0, 1, &LCD_stk[2][STACK_SIZE_LCD-1], STACK_SIZE_LCD);
		  l_racketId = CoCreateTask (LCDRacket0ControlTask, 0, 1, &LCD_stk[3][STACK_SIZE_LCD-1], STACK_SIZE_LCD);
		  r_racketId = CoCreateTask (LCDRacket1ControlTask, 0, 1, &LCD_stk[4][STACK_SIZE_LCD-1], STACK_SIZE_LCD);
		  waitForKey(0, 0);
		  CoDelTask(ballId);
		  CoDelTask(l_racketId);
		  CoDelTask(r_racketId);

	  }
	  game_overId = CoCreateTask (gameOverTask, 0, 1, &LCD_stk[5][STACK_SIZE_LCD-1], STACK_SIZE_LCD);
	  waitForKey(0, 0);
	  CoDelTask(game_overId);

  }
}

void gameOverTask(void * parg){
	int col, fil, i;
	char array_game_over[] = "Game Over ";

	char str_winner[] = "Player X wins";
	if (score0 > 1){
		str_winner[7] = '1';
	} else{
		str_winner[7] = '2';
	}

	LCD_PrintText(100, 110, str_winner, White, Black);

	i = 0;
	col = 0;
	fil = 0;

	for(;;){
		LCD_PutChar(col*8,fil*16,array_game_over[i],White,Black);
		i++;
		if (i > 9) i = 0;

		col++;

		if(col > 39) {
			col = 0;
			fil ++;
		}

		if (fil > 14){
			LCD_Clear(Black);
			fil = 0;
		}
		CoTimeDelay(0,0,0,40);
	}
}

void LCDInitScreenControl(void * parg) {

	char texto[] = "Ainhoa y Pablo";

	LCD_Clear(Black);

	// Letra P
	LCD_FillRectangle(32, 43, 44, 44, White);
	LCD_FillRectangle(43, 54, 22, 22, Black);
	LCD_FillRectangle(32, 76, 11, 44, White);

	// Letra O
	LCD_FillRectangle(98, 65, 44, 55, White);
	LCD_FillRectangle(109, 76, 22, 33, Black);

	// Letra N
	LCD_FillRectangle(164, 65, 11, 55, White);
	LCD_FillRectangle(175, 76, 11, 11, White);
	LCD_FillRectangle(186, 87, 11, 11, White);
	LCD_FillRectangle(197, 98, 11, 11, White);
	LCD_FillRectangle(208, 65, 11, 55, White);

	//Letra G
	LCD_FillRectangle(241,  65, 44, 11, White);
	LCD_FillRectangle(241,  76, 11, 33, White);
	LCD_FillRectangle(241, 109, 44, 11, White);
	LCD_FillRectangle(263,  87, 22, 11, White);
	LCD_FillRectangle(274,  98, 11, 11, White);

	LCD_PrintText(32, 142, texto, White, Black);

	for(;;) {

	}
}

void InitScreen4Game(){
	uint16_t ballPositionX = pos_init;
	uint8_t ballPositionY = 11;

	LCD_Clear(Black);

	LCD_FillCircle(ballPositionX, ballPositionY, 8, White);
	LCD_FillRectangle(0, 0, racket_width, racket_height, White);
	LCD_FillRectangle(311, 0, racket_width, racket_height, White);
	LCD_FillRectangle(158, 0, 2, 240, White);
	printScoreTable();

}

void LCDBallControlTask(void * parg) {

	uint16_t ballPositionX = pos_init, ballPositionY = 11;
	uint16_t prev_ballPositionX, prev_ballPositionY;

	uint8_t directionX = directionX_right;
	uint8_t directionY = directionY_down;

	uint8_t pos_racket0, pos_racket1;


	for(;;){

		prev_ballPositionX = ballPositionX;
		prev_ballPositionY = ballPositionY;

		if(directionX == directionX_right) ballPositionX += 2;
		else ballPositionX -= 2;

		if(directionY == directionY_down) ballPositionY++;
		else ballPositionY--;

		if (ballPositionY == top_limit && directionY == directionY_up) directionY = directionY_down;
		if (ballPositionY == low_limit && directionY == directionY_down) directionY = directionY_up;


		if (ballPositionX == left_limit && directionX == directionX_left) {
			CoPendSem(racket0_sem,0);
				pos_racket0 = global_racket0_posY;
			CoPostSem(racket0_sem);

			if((pos_racket0-7 < ballPositionY) && (ballPositionY < pos_racket0+47))
				directionX = directionX_right;
			else {
				score1++;
				printScoreTable();
				for(;;);
			}

		}
		if (ballPositionX == right_limit && directionX == directionX_right) {
			CoPendSem(racket1_sem, 0);
				pos_racket1 = global_racket1_posY;
			CoPostSem(racket1_sem);

			if((pos_racket1-7 < ballPositionY) && (ballPositionY < pos_racket1+47))
				directionX = directionX_left;
			else{
				score0++;
				printScoreTable();
				for(;;);
			}
		}

		LCD_FillCircle(prev_ballPositionX, prev_ballPositionY, 8, Black);
		printScoreTable();
		LCD_FillCircle(ballPositionX, ballPositionY, 8, White);
		LCD_FillRectangle(158, 0, 2, 240, White);

		CoTimeDelay(0,0,0,4);


	}

}

void printScoreTable(){
	char str_score0[1], str_score1[1];

	sprintf(str_score0, "%d", score0);
	sprintf(str_score1, "%d", score1);
	LCD_PrintText(138, 10, str_score0, White, Black);
	LCD_PrintText(170, 10, str_score1, White, Black);

}

void LCDRacket0ControlTask(void * parg){

	int racketPositionY = 0;
	int prev_racketPositionY = 0;
	uint16_t directionY = 0;



	for(;;){

		prev_racketPositionY = racketPositionY;


		directionY = getAnalogJoy(1);

		if (directionY > 3000){ // Upwards

			racketPositionY-=4;

			if (racketPositionY <= 0)racketPositionY = 0;
			else{
				LCD_FillRectangle(0, prev_racketPositionY, racket_width, racket_height, Black);
				LCD_FillRectangle(0, racketPositionY, racket_width, racket_height, White);
			}

		} else if (directionY < 1000){ // Downwards

			racketPositionY+=4;

			if (racketPositionY >= 239-racket_height) racketPositionY = 239-racket_height;
			else{
				LCD_FillRectangle(0, prev_racketPositionY, racket_width, racket_height, Black);
				LCD_FillRectangle(0, racketPositionY, racket_width, racket_height, White);
			}
		}

		CoPendSem(racket0_sem,0);
			global_racket0_posY = racketPositionY;
		CoPostSem(racket0_sem);

		CoTimeDelay(0,0,0,10);

	}
}


void LCDRacket1ControlTask(void * parg){

	int racketPositionY = 0;
	int prev_racketPositionY = 0;
	uint16_t directionY = 0;

	for(;;){

		prev_racketPositionY = racketPositionY;

		directionY = getAnalogJoy(0);

		if (directionY > 3000){ // Upwards

			racketPositionY-=4;

			if (racketPositionY <= 0)racketPositionY = 0;
			else{
				LCD_FillRectangle(311, prev_racketPositionY, racket_width, racket_height, Black);
				LCD_FillRectangle(311, racketPositionY, racket_width, racket_height, White);
			}

		} else if (directionY < 1000){ // Downwards

			racketPositionY+=4;

			if (racketPositionY >= 239-racket_height) racketPositionY = 239-racket_height;
			else{
				LCD_FillRectangle(311, prev_racketPositionY, racket_width, racket_height, Black);
				LCD_FillRectangle(311, racketPositionY, racket_width, racket_height, White);
			}
		}

		CoPendSem(racket1_sem,0);
			global_racket1_posY = racketPositionY;
		CoPostSem(racket1_sem);

		CoTimeDelay(0,0,0,10);

	}
}


