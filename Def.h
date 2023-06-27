
#ifndef DEF_H
#define	DEF_H

#include<stdio.h>
#include "init.h"
#include "inter.h"
#include "MI2C.h"
#include "functions.h"


extern unsigned char j;

extern unsigned char marche;                                    // Booléen de marche/arrêt
extern unsigned char distance;                                  // Variable distance lue par le sonar
extern unsigned char mode;                                      // Phase du programme
extern unsigned char led;

#endif	/* DEF_H */

