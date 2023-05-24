
#ifndef DEF_H
#define	DEF_H


#include "init.h"
#include "inter.h"
#include "MI2C.h"
#include "functions.h"

void write(char c);
void print(char *string);

extern unsigned char j;

extern unsigned char marche;                                    // Booléen de marche/arrêt
extern unsigned char distance;                                  // Variable distance lue par le sonar
extern unsigned char mode;                                      // Phase du programme

#endif	/* DEF_H */

