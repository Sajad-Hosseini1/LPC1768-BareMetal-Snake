
#include "rand.h"

#include <stdlib.h>
#include <time.h>

extern int index;

uint32_t random(uint32_t max) {	
    static uint32_t seed = 0; 
    if (seed == 0) {
        seed = LPC_TIM0->TC; 
    }
    seed = (1103515245 * seed + 12345) & 0x7FFFFFFF;
    return seed % max;
}

void rand_in_range() {
    index = random(418) + 31; 
}