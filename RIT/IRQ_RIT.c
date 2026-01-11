#include "LPC17xx.h"
#include "RIT.h"

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

extern Direction Snake_dir;
extern int Pdir;

void RIT_IRQHandler(void) {

    uint32_t joy = LPC_GPIO1->FIOPIN >> 26;


    if((joy & 0x1) == 0 && Pdir != UP)     
        Snake_dir = DOWN;
			
    else if((joy & 0x2) == 0 && Pdir != RIGHT)
        Snake_dir = LEFT;

    else if((joy & 0x4) == 0 && Pdir != LEFT)
        Snake_dir = RIGHT;

    else if((joy & 0x8) == 0 && Pdir != DOWN)  
        Snake_dir = UP;
			  
    LPC_RIT->RICTRL |= 1;
	
}
