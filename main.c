#include<p18f2520.h>
#include<stdlib.h>
#include "Def.h"

unsigned char i=0;
unsigned char j;
char message[] = "Entrer vitesse: ";
char messageBat[] = "Batterie: ";

unsigned char batterie[8] = {255,255,255,255,255,255,255,255};
unsigned char compteurBat = 0;
unsigned char indiceBat = 0;
unsigned int tensionBat = 123;
unsigned char compteurSon = 0;
unsigned char compteur = 0;
unsigned char UBat;


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
            // Moyenne glissante
            compteurBat = 0;
            batterie[indiceBat] = UBat;
            indiceBat ++;
            indiceBat %= 8;
            tensionBat = 0;
            for(j=0;j<8;j++){
                tensionBat += batterie[j];
            }
            tensionBat/=8;
            ADCON0bits.GO=1; //Lancer la conversion

            // Allumage de la LED
            if(tensionBat < 150){
                LATBbits.LATB5 = 1;
            }
            
            // Affichage
            print(messageBat);
            tensionBat = (tensionBat + 14) / 16; // Conversion en V
            write(tensionBat/10+48);
            write(tensionBat%10+48);
            write('V');
            write('\r\n');
        }
        else{
            compteurBat++;
        }
        if(PIR1bits.ADIF){
        PIR1bits.ADIF=0;
        UBat=ADRESH;

    }
        if(PIR1bits.ADIF){
        PIR1bits.ADIF=0;
        UBat=ADRESH;

    }
        // Compteur Sonar
        if(compteurSon == 10){
            //LATBbits.LATB5=~LATBbits.LATB5;
            write('S');
            write('\r\n');
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

/* Configuration Oscillateur*/
    initHorloge();
    initCAN();

/* Configuration TIMER2 */
    T2CONbits.T2CKPS1= 1;  // Prescaler = 16

/* R?glage p?riode*/
	 // Reglage periode FPWM = Fosc/(4*(PR2+1)*PRE)
    PR2=124;

/* Reglage rapport cyclique */
    //20%--> 100, 50%--> 250
    CCPR1L=250>>2;
    CCPR2L=250>>2;


    CCP1CONbits.DC1B=0;
    CCP2CONbits.DC2B=0;

    //UART
    BAUDCONbits.BRG16 = 0;
    TXSTAbits.BRGH = 0;
    TXSTAbits.SYNC = 0;
    SPBRGH = 0;
    SPBRG = 25; //baudrate 4808 au lieu de 4800


/* Configuration E/S*/
    TRISCbits.RC2=0; //Moteur
    TRISCbits.RC1=0;

    TRISCbits.RC6 = 1; //RX

    TRISBbits.RB5 = 0;//LED

    TRISBbits.RB0 = 1; //INT0

/* Selection Mode PWM */

    CCP1CONbits.CCP1M=12;
    CCP2CONbits.CCP2M=12;

/* Reglage POST SCALEr*/
    T2CONbits.T2OUTPS=9;

/* TMR2 marche */
    T2CONbits.TMR2ON=1;


    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;

/* Interrruption*/
    PIE1bits.TMR2IE=1;  // Validation IT TMR2
    IPR1bits.TMR2IP=1;  // Priorit? haute inutile (voir doc page 102)

    PIE1bits.TXIE = 1;  //Valider les IT pour UART
    PIR1bits.TXIF = 0;

    INTCON2bits.INTEDG0 = 0; // Interruption front descendant
    INTCONbits.INT0IE = 1; // Validation INT0

    INTCONbits.PEIE = 1;  // Validation INT p?riph?rique
    INTCONbits.GIE = 1;  // Validation Globale des interruptions

    while(1){

    }
}
