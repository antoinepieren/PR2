#include<p18f2520.h>
#include<stdlib.h>
#include "Def.h"
#pragma config OSC = INTIO67
#pragma config PBADEN = OFF, WDT = OFF, LVP = OFF, DEBUG = ON

unsigned char j;
char messageInit[] = "Fin init";
unsigned char marche = 0;                                       // Booléen de marche/arrêt
//unsigned char led = 0b00000000;
unsigned char distance;                                         // Variable distance lue par le sonar
unsigned char mode = 0;                                         // Phase du programme
unsigned char pwm_on = 62;                                      // Rapport cyclique pwm (250>>2)

void write(char c){
    while(PIE1bits.TXIE);
    TXREG = c;
    while(!PIR1bits.TXIF);
}

void print(char *string){
    j = 0;
    while(string[j] != '\0'){
        write(string[j]);
        j++;
    }
}

void main(void) {

    init();
    print(messageInit);
    write('\r\n');
    Write_PCF8574(0x40, 0);
    while(1){
        if(marche == 1){ // Programme du mouvement qui s'arrête après appui sur le bouton du milieu de la telecommande
            if(mode == 2){
                if(distance > 50){
                    CCPR1L = pwm_on;
                    CCPR2L = pwm_on;
                }
                else{
                    CCPR1L = 0;
                    CCPR2L = 0;
                }
            }
            else{
                if(distance > 40 && distance < 150){
                    mode = 1;
                    CCPR1L = pwm_on;
                    CCPR2L = pwm_on;
                }
                else{
                    if(mode == 1){
                        mode = 2;
                        CCPR1L = 0;
                        CCPR2L = 0;
                    }
                    else{};
                }
            }
        }
        else{ // Arrêt des moteurs après appui sur le bouton du milieu de la telecommande
            CCPR1L = 0;
            CCPR2L = 0;
        }

    }
}
