#include <p18f2520.h>
#include "init.h"

void init(void){
    OSCCONbits.IRCF = 7; // 8Mhz

    TRISBbits.RB5 = 0;//LED

    TRISBbits.RB0 = 1; //INT0
    PORTBbits.RB1 = 1; //IR OFF
    
    initTimer();
    initCAN();
    initUART();
    MI2CInit();

    TRISAbits.RA6 = 0; //DIRD sortie
    TRISAbits.RA7 = 0; //DIRG sotie
    PORTAbits.RA6 = 1; //DRID en avance
    PORTAbits.RA7 = 1; //DIRG en avance

    INTCON2bits.INTEDG0 = 0; // Interruption front descendant INT0
    INTCONbits.INT0IE = 1; // Validation INT0

    INTCONbits.PEIE = 1;  // Validation INT peripherique
    INTCONbits.GIE = 1;  // Validation Globale des interruptions

}

void initUART(void){
    //UART
    TRISCbits.RC6 = 1; //RX

    BAUDCONbits.BRG16 = 0;
    TXSTAbits.BRGH = 0;
    TXSTAbits.SYNC = 0;
    SPBRGH = 0;
    SPBRG = 25; //baudrate 4808 au lieu de 4800

    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
    PIE1bits.TXIE = 1;  //Valider les IT pour UART
    PIR1bits.TXIF = 0;
}

void initCAN(void){
    /* Configuration E/S*/
	TRISBbits.TRISB5=0;	// RB5 en sortie LED TEST
/*Configuration entrée analogique*/
    ADCON1bits.PCFG=12;

/*Sélection Vref+,Vref-*/
    ADCON1bits.VCFG=0; // inutile configuration par defaut
/*Choix TAD*/
    ADCON2bits.ADCS=1; //N=8, T_AD = 1us
/*T acquisition*/
    ADCON2bits.ACQT=3;  // Temps acq = 6ï¿½s > 4.2ï¿½s
 /* Choix AN2*/
    ADCON0bits.CHS=2;
 /*Justification à gauche*/
    ADCON2bits.ADFM=0;
 /*Validation ADC (dï¿½marage pï¿½riphï¿½rique)*/
    ADCON0bits.ADON=1;
  /*Interruption*/
    PIR1bits.ADIF=0; // Effacement
    PIE1bits.ADIE=1;// Demasquage
    INTCONbits.PEIE=1; // Val pï¿½riphï¿½rique
}


void initTimer(void){
    TRISCbits.RC1 = 0; // PWMD en output
    TRISCbits.RC2 = 0; // PWMG en output
    T2CONbits.T2CKPS1 = 1; // Prescaler 16
    PR2 = 124;  // Pï¿½riode de 1ms
    CCPR1L = 0;
    CCPR2L = 0;

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

    /* Reglage POST SCALEr*/
    T2CONbits.T2OUTPS=9;


    T2CONbits.TMR2ON = 1; // Validation timer 2

    PIE1bits.TMR2IE=1;  // Validation IT TMR2
    IPR1bits.TMR2IP=1;  // Priorit? haute inutile (voir doc page 102)

   

}
