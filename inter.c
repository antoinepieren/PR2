#include <p18f2520.h>
#include "inter.h"
#include "Def.h"

// Messages Uart

char messageBat[] = "Batterie:";
char messageMode[] = "Mode:";
char messageSonar[] = "Sonar:";


unsigned char batterie[8] = {255,255,255,255,255,255,255,255};  // Tableau moyenne glissante batterie
unsigned char compteurBat = 0;                                  // Compteur jusqu'à 100 (1s)
unsigned char indiceBat = 0;                                    // Indice dans batterie pour moyenne glissante
unsigned int tensionBat;                                        // Variable affichage batterie
unsigned char compteurSon = 0;                                  // Compteur jusqu'à 10 (100ms)
unsigned char compteurTel = 0;                                  // Compteur télécommande (2s)
unsigned char UBat;                                             // Tension instantanée batterie

unsigned char touche[3];                                        // Touche télécommande



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
            compteurBat = 0;
            ADCON0bits.GO=1; //Lancer la conversion
        }
        else{
            compteurBat++;
        }

        // Compteur Sonar
        if(compteurSon == 10 && marche == 1){
            compteurSon = 0;
            distance = SONAR_Read(0xE0, 0x02);
            SONAR_Write(0xE0, 0x00);
            SONAR_Write(0xE0, 0x51);

            // Affichage sonar uart
            /*
            print(messageSonar);
            write(distance/100+48);
            write((distance%100)/10+48);
            write(distance%10+48);
            //write('\r\n');
            */
            // Affichage
            /*
            print(messageMode); // Affichage mode
            write(mode%10+48);
            */
            printf("Mode : %d\r\n",mode);
            //write('\r\n');
            


        }
        else{
            compteurSon++;
        }

        // Compteur télécommande
        if (compteurTel > 0){
          compteurTel ++;
          if (compteurTel > 50){ // 500 millisecondes pour éviter les appuis répétés
            compteurTel = 0;
          }
        }


    }

    //UART
    
    if(PIR1bits.TXIF && PIE1bits.TXIE)
    {
        PIE1bits.TXIE = 0;
    }

    //Surveillance Batterie
    if(PIR1bits.ADIF){
            PIR1bits.ADIF=0;
            UBat=ADRESH;

            // Moyenne glissante
            // Update tableau batterie
            batterie[indiceBat] = UBat;
            indiceBat ++;
            indiceBat %= 8;

            // Moyenne
            tensionBat = 0;
            for(j=0;j<8;j++){
                tensionBat += batterie[j];
            }
            tensionBat = tensionBat >> 3;

            // Allumage de la LED
            if(tensionBat < 150){
                LATBbits.LATB5 = 1;
            }

            // Affichage uart (pour les simulations)
            /*
            print(messageBat);
            write(tensionBat/100+48);
            write((tensionBat%100)/10+48);
            write(tensionBat%10+48);
            write(':');
            tensionBat = (tensionBat + 14) / 16; // Conversion en V
            write(tensionBat/10+48);
            write(tensionBat%10+48);
            write('V');
            */
            //write('\r\n');
            tensionBat = (tensionBat + 14) / 16;
            printf("Batterie : %dV\r\n",tensionBat);
            

            led = distance; // L'affichage de ~led se fait dans la boucle main
        }

    // Télécommande
    if(INTCONbits.INT0IF)      //INT0
    {
      INTCONbits.INT0IF = 0;
      if (compteurTel == 0){
        compteurTel = 1;
        
        while(Detecte_i2c(0xA2));
        Lire_i2c_Telecom(0xA2, touche);
        if(touche[1]==0x33){ // Touche du milieu
          if(marche == 0){
              marche = 1;
          }
          else{
              marche = 0;
          }
        }
      }
    }
}

