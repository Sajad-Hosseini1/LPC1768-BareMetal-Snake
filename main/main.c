#include "LPC17xx.h"
#include "led.h"
#include "joystick.h"
#include "RIT.h"
#include "GLCD.h" 
#include <stdio.h>
#include "timer.h"
#include <stdlib.h>
#include <time.h>
#include "button.h"
#include <stdbool.h>
#include "rand.h"

typedef enum {   // For Direction of the Snake
    UP,
    DOWN,
    LEFT,
    RIGHT			
} Direction;									 
Direction Snake_dir = RIGHT;	 // Begin with right


#define ROWS 17		
#define COLS 30

void init_drawing(void);
void SnakeM(Direction Dir);
void delay_ms(uint32_t ms);
void print_Score(void);

uint8_t SnakeMap[ROWS * COLS] = {			//Primary Map of the game
'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',
'#','@',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
'#',' ',' ',' ',' ',' ','*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',
'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',
};


int SnakeP = 0;	
int snake_length = 1;
int snake_body[100];
bool grow = false;
char Pdir;
int index;
bool flag=true;


int main(){
	SystemInit();
	BUTTON_init();
	for(int i=0;i<ROWS * COLS;i++){											
		if(SnakeMap[i] == '@'){SnakeP = i;break;}		 //For Finding Snake head	
	}
	
	snake_body[0]= SnakeP;    //Head of the Snake == Position of the snake
	joystick_init(); 
	init_RIT(0xFFF);
	init_timer(0,0xFFFFF);
  LCD_Initialization();
	LCD_Clear(Black);	
	init_drawing();
	enable_timer(0);
	enable_RIT();
	
	
	
	
  while (1)	
  {
		SnakeM(Snake_dir);
		delay_ms(1);
	
	}	
	
}


void init_drawing(void) {   //Drawing Map in the Emulator
	GUI_Text(100, 0, (uint8_t *)"Score", White, Black);
	print_Score();
	for(int i=0;i<ROWS;i++){
		for(int j=0;j<COLS;j++){
			uint8_t character = SnakeMap[i*30+j]; 
			if(character == '#')PutChar(j*8,32+(i*16),character,Cyan,Black);		
			if(character == ' ')PutChar(j*8,32+(i*16),character,Black,Black);
			if(character == '@')PutChar(j*8,32+(i*16),character,Yellow,Black);			
			if(character == '*')PutChar(j*8,32+(i*16),character,Yellow,Black);
		}
	}
}

void print_Score(void){
	char buffer[5];
	sprintf(buffer, "%04d", snake_length-1);					
	GUI_Text(100, 16, (uint8_t *)buffer, White, Black);		
}


void SnakeM(Direction Dir){ 
	
	int tail;
	int next_pos = 0;
	
	if(flag)
	{
	switch (Dir) {									
		case UP:											
				Pdir= Snake_dir;
				next_pos = SnakeP - COLS;	//next pos = Upper Column
				if(SnakeMap[next_pos] == '#') //If next pos is wall or snake's body = game over
				{
					GUI_Text(80, 152,   (uint8_t *)"Game Over!!!", Green, White);
					disable_RIT();
					disable_timer(0);
					flag= false;
					break;
				}
				
				 tail = snake_body[snake_length - 1]; //tail is equal to last piece of body

				 for(int i = snake_length - 1; i > 0; i--)
				{
					snake_body[i] = snake_body[i - 1];
						
				}

					snake_body[0] = next_pos; 
					SnakeP = next_pos;
						
				 if(SnakeMap[next_pos] == '*') //if next pos is equal to * snake can grow
				{
					grow = true;
				}

				 if(!grow) //if snake can not grow, tail must be removed
				{
					SnakeMap[tail] = ' ';
					update_display(tail, ' ', 0x0000);
				}


				 if(grow && snake_length < 100) //if snake can grow => next pos = snake's head, increase length of the snake, add snake's body
				{			
					SnakeMap[next_pos]= '@';
					update_display(next_pos, '@', 0x07E0);
					grow = false;
					snake_length++;
					print_Score();
					snake_body[snake_length - 1] = tail;
					for(int i = snake_length - 1; i > 0; i--) { 
					SnakeMap[snake_body[i]] = '#';
					update_display(snake_body[i], '#', 0xFFFF);
				}
							
					rand_in_range();
					while(SnakeMap[index] == '@' || SnakeMap[index] == '#') //if next food must be in the free space
				{
					rand_in_range();
					SnakeMap[index] = '*';
				}
					SnakeMap[index] = '*';
					update_display(index, '*', 0xFFE0);				
				}
	
					for(int i = snake_length - 1; i > 0; i--) //drawing snake's head and snake's body
				{
					SnakeMap[snake_body[i]] = '#';
					update_display(snake_body[i], '#', 0xFFFF);
				}
					SnakeMap[snake_body[0]] = '@';
					update_display(snake_body[0], '@', 0x07E0);
				
				break;
		case DOWN:
			Pdir= Snake_dir;
				next_pos = SnakeP + COLS;  //next pos = Down column
				if(SnakeMap[next_pos] == '#') //If next pos is wall or snake's body = game over
				{
					GUI_Text(80, 152,   (uint8_t *)"Game Over!!!", Green, White);
					disable_RIT();
					disable_timer(0);
					flag= false;
					break;
				}
				
				 tail = snake_body[snake_length - 1]; //tail is equal to last piece of body

				 for(int i = snake_length - 1; i > 0; i--)
				{
					snake_body[i] = snake_body[i - 1];
						
				}

					snake_body[0] = next_pos; 
					SnakeP = next_pos;
						
				 if(SnakeMap[next_pos] == '*') //if next pos is equal to * snake can grow
				{
					grow = true;
				}

				 if(!grow) //if snake can not grow, tail must be removed
				{
					SnakeMap[tail] = ' ';
					update_display(tail, ' ', 0x0000);
				}


				 if(grow && snake_length < 100) //if snake can grow => next pos = snake's head, increase length of the snake, add snake's body
				{			
					SnakeMap[next_pos]= '@';
					update_display(next_pos, '@', 0x07E0);
					grow = false;
					snake_length++;
					print_Score();
					snake_body[snake_length - 1] = tail;
					for(int i = snake_length - 1; i > 0; i--) { 
					SnakeMap[snake_body[i]] = '#';
					update_display(snake_body[i], '#', 0xFFFF);
				}
							
					rand_in_range();
					while(SnakeMap[index] == '@' || SnakeMap[index] == '#') //if next food must be in the free space
				{
					rand_in_range();
					SnakeMap[index] = '*';
				}
					SnakeMap[index] = '*';
					update_display(index, '*', 0xFFE0);				
				}
	
					for(int i = snake_length - 1; i > 0; i--) //drawing snake's head and snake's body
				{
					SnakeMap[snake_body[i]] = '#';
					update_display(snake_body[i], '#', 0xFFFF);
				}
					SnakeMap[snake_body[0]] = '@';
					update_display(snake_body[0], '@', 0x07E0);
						
				break;
		case LEFT:		//next pos = left row
				Pdir= Snake_dir;
				next_pos = SnakeP - 1;
				if(SnakeMap[next_pos] == '#') //If next pos is wall or snake's body = game over
				{
					GUI_Text(80, 152,   (uint8_t *)"Game Over!!!", Green, White);
					disable_RIT();
					disable_timer(0);
					flag= false;
					break;
				}
				
				 tail = snake_body[snake_length - 1]; //tail is equal to last piece of body

				 for(int i = snake_length - 1; i > 0; i--)
				{
					snake_body[i] = snake_body[i - 1];
						
				}

					snake_body[0] = next_pos; 
					SnakeP = next_pos;
						
				 if(SnakeMap[next_pos] == '*') //if next pos is equal to * snake can grow
				{
					grow = true;
				}

				 if(!grow) //if snake can not grow, tail must be removed
				{
					SnakeMap[tail] = ' ';
					update_display(tail, ' ', 0x0000);
				}


				 if(grow && snake_length < 100) //if snake can grow => next pos = snake's head, increase length of the snake, add snake's body
				{			
					SnakeMap[next_pos]= '@';
					update_display(next_pos, '@', 0x07E0);
					grow = false;
					snake_length++;
					print_Score();
					snake_body[snake_length - 1] = tail;
					for(int i = snake_length - 1; i > 0; i--) { 
					SnakeMap[snake_body[i]] = '#';
					update_display(snake_body[i], '#', 0xFFFF);
				}
							
					rand_in_range();
					while(SnakeMap[index] == '@' || SnakeMap[index] == '#') //if next food must be in the free space
				{
					rand_in_range();
					SnakeMap[index] = '*';
				}
					SnakeMap[index] = '*';
					update_display(index, '*', 0xFFE0);				
				}
	
					for(int i = snake_length - 1; i > 0; i--) //drawing snake's head and snake's body
				{
					SnakeMap[snake_body[i]] = '#';
					update_display(snake_body[i], '#', 0xFFFF);
				}
					SnakeMap[snake_body[0]] = '@';
					update_display(snake_body[0], '@', 0x07E0);
				
				break;
		case RIGHT:	
				Pdir= Snake_dir;			
				next_pos = SnakeP + 1; //next pos = right row
				if(SnakeMap[next_pos] == '#') //If next pos is wall or snake's body = game over
				{
					GUI_Text(80, 152,   (uint8_t *)"Game Over!!!", Green, White);
					disable_RIT();
					disable_timer(0);
					flag= false;
					break;
				}
				
				 tail = snake_body[snake_length - 1]; //tail is equal to last piece of body

				 for(int i = snake_length - 1; i > 0; i--)
				{
					snake_body[i] = snake_body[i - 1];
						
				}

					snake_body[0] = next_pos; 
					SnakeP = next_pos;
						
				 if(SnakeMap[next_pos] == '*') //if next pos is equal to * snake can grow
				{
					grow = true;
				}

				 if(!grow) //if snake can not grow, tail must be removed
				{
					SnakeMap[tail] = ' ';
					update_display(tail, ' ', 0x0000);
				}


				 if(grow && snake_length < 100) //if snake can grow => next pos = snake's head, increase length of the snake, add snake's body
				{			
					SnakeMap[next_pos]= '@';
					update_display(next_pos, '@', 0x07E0);
					grow = false;
					snake_length++;
					print_Score();
					snake_body[snake_length - 1] = tail;
					for(int i = snake_length - 1; i > 0; i--) { 
					SnakeMap[snake_body[i]] = '#';
					update_display(snake_body[i], '#', 0xFFFF);
				}
							
					rand_in_range();
					while(SnakeMap[index] == '@' || SnakeMap[index] == '#') //if next food must be in the free space
				{
					rand_in_range();
					SnakeMap[index] = '*';
				}
					SnakeMap[index] = '*';
					update_display(index, '*', 0xFFE0);				
				}
	
					for(int i = snake_length - 1; i > 0; i--) //drawing snake's head and snake's body
				{
					SnakeMap[snake_body[i]] = '#';
					update_display(snake_body[i], '#', 0xFFFF);
				}
					SnakeMap[snake_body[0]] = '@';
					update_display(snake_body[0], '@', 0x07E0);
				
				break;
	}
}	
}


void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms; i++) {
        for(volatile uint32_t j = 0; j < 8000; j++);
    }
}