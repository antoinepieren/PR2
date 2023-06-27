#include<p18f2520.h>
#include<stdlib.h>
#include "Def.h"
#pragma config OSC = INTIO67
#pragma config PBADEN = OFF, WDT = OFF, LVP = OFF, DEBUG = ON

unsigned char j;
unsigned char marche;                                       // Booléen de marche/arrêt
unsigned char led;                                          // Variable pour l'affichage des leds
unsigned char distance;                                     // Variable distance lue par le sonar
unsigned char mode;                                         // Phase du programme
unsigned char pwm_on;                                       // Rapport cyclique pwm (mesuré à 26.4 cm/s)

void main(void) {
    marche = 0;
    led = 0;
    mode = 0;
    pwm_on = 50;
    
    init();
    printf("Fin init\r\n");
    
    while(1){
        Write_PCF8574(0x40, ~led);
        CCPR1L = pwm_on;
        CCPR2L = pwm_on;
        if(marche == 1){ // Programme du mouvement qui s'arrête après appui sur le bouton du milieu de la telecommande
            if(mode == 2){
                if(distance > 50 && distance < 150){
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
            led = 0;
        }
    }
}
