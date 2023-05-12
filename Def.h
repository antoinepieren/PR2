
#ifndef DEF_H
#define	DEF_H

#pragma config OSC = INTIO67
#pragma config PBADEN = OFF, WDT = OFF, LVP = OFF, DEBUG = ON
#include "init.h"
#include "inter.h"
//#include "MI2C.h"

void write(char c);
void print(char *string);

#endif	/* DEF_H */

