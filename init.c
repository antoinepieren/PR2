#include <p18f2520.h>
#include "init.h"

void init(void){
    //initHorloge();
    //initTimer();
    //initCAN();
    //MI2CInit();
    char init = 0;
}

void initHorloge(void){
    OSCCONbits.IRCF = 7; // 8Mhz
}

void initTimer(void){
    TRISCbits.RC1 = 0; // PWMD en output
    TRISCbits.RC2 = 0; // PWMG en output
    T2CONbits.T2CKPS1 = 1; // Prescaler 16
    PR2 = 124;  // P�riode de 1ms
    CCPR1L = 50; //TODO :  Rapport cyclique � d�terminer exp�rimentalement (formule page 119 du poly)
    CCPR2L = 50;

    // 2 derniers bits du rapport cyclique
    // PWM1 = RC2 = PWMG
    CCP1CONbits.DC1B0 = 0;
    CCP1CONbits.DC1B1 = 0;
    // PWM2 = RC1 = PWMD
    CCP2CONbits.DC2B0 = 0;
    CCP2CONbits.DC2B1 = 0;

    // Mode PWM page 149 de la datasheet en pdf
    // PWM1 = RC2 = PWMG
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
    // PWM2 = RC1 = PWMD
    CCP2CONbits.CCP2M3 = 1;
    CCP2CONbits.CCP2M2 = 1;
    
    T2CONbits.TMR2ON = 1; // Validation timer 2
}
