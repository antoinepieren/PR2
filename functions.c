#include <p18f2520.h>
#include "functions.h"

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