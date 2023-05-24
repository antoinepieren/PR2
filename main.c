#include<p18f2520.h>
#include<stdlib.h>
#include "Def.h"
#pragma config OSC = INTIO67
#pragma config PBADEN = OFF, WDT = OFF, LVP = OFF, DEBUG = ON

char message[] = "Entrer vitesse: ";
unsigned char i=0;
unsigned char compteurBat = 0;
unsigned char compteurSon = 0;
unsigned char compteur = 0;
unsigned char j;

void HighISR(void);


#pragma code HighVector=0x08
void IntHighVector(void)
{
    _asm goto HighISR _endasm
}
#pragma code

#pragma interrupt HighISR
void HighISR(void)
{
    if(PIR1bits.TMR2IF) //Interruption TIMER2 + compteur
    {
        PIR1bits.TMR2IF=0; // Validation interruption
        // Compteur Batterie
        if(compteurBat == 100){
            LATBbits.LATB5=~LATBbits.LATB5;
            compteurBat = 0;
        }
        else{
            compteurBat++;
        }
        // Compteur Sonar
        if(compteurSon == 10){
            LATBbits.LATB5=~LATBbits.LATB5;
            compteurSon = 0;
        }
        else{
            compteurSon++;
        }

    }

    if(PIR1bits.TXIF && PIE1bits.TXIE)
    {
        PIE1bits.TXIE = 0;
    }

    if(INTCONbits.INT0IF)      //INT0
    {
      INTCONbits.INT0IF = 0;

      //LATBbits.LATB5=~LATBbits.LATB5;
    }

}

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
