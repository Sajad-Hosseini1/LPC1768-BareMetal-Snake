#include "button.h"
#include "LPC17xx.h"
//#include <stdbool.h>
//typedef enum {
//    UP,
//    DOWN,
//    LEFT,
//    RIGHT,
//    STOP
//} Direction;
typedef enum {
    Pause,
    Resume,
		Victory,
		Over
} State;
extern State Status;
//extern Direction PacMan_dir;
//bool flag = 1;
extern void pause_game(void);
extern void Resume_game(void);

extern void init_drawing(void);
extern void update_display(int position,uint8_t ch);


void EINT0_IRQHandler (void)	  
{

//			if(Status == Resume ){
//				Status = Pause;
//				pause_game();
//			}
//			else if(Status == Pause){
//				Status = Resume;
//				init_drawing(); //same as resume();
//			}		
	
  LPC_SC->EXTINT |= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  
{

	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  
{

  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


