/*********************************************************************************************************
**CountDown time of the game
**While showing on screen
**if time is ove ---> shows GameOver
**
**Also compute time for replacing PowerPills (each 10 sec)
*********************************************************************************************************/
#include "LPC17xx.h"
#include "timer.h"
#include "RIT/RIT.h"
#include "GLCD.h"
#include <stdio.h>


char CountDown = 60;
void print_CountDown(void);
void Over_game(void);

extern void placePowerPills();

typedef enum {
    Pause,
    Resume,
		Victory,
		Over
} State;
extern State Status;
void TIMER0_IRQHandler (void)
{
//	if(Status == Resume){		//if state is resume, contdown time
//		CountDown--;
//		if(CountDown == 0){		//it it reached 0 -> GameOver
//			Over_game();
//			}
//		else{
//				if(CountDown%10 == 0){ //each 10s replace Powerpills
//				placePowerPills();
//				}
//			}
//		print_CountDown();			//print time
//		}

  LPC_TIM0->IR |= 1;			/* clear interrupt flag */
  return;
}


void Over_game(void){
//	Status = Over;
//	LCD_Clear(Black);
//	GUI_Text(76, 152,   (uint8_t *)"Game Over:(", White, Black);
//	disable_RIT();
//	disable_timer(0);
}


void print_CountDown(void){
	char buffer[4];
	sprintf(buffer, "%02d", CountDown);									//to change Char(number) to Character LCD can show (on buffer)
	GUI_Text(2, 16, (uint8_t *)buffer, White, Black); 	//show timer time
}

