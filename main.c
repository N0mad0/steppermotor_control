// PIC16F88 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1
#pragma config FOSC = INTOSCIO       // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CCPMX = RB0      // CCP1 Pin Selection bit (CCP1 function on RB0)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// CONFIG2
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 8000000

void delay(int t) {
  for(int i = 0; i < t; i++) {
    __delay_us(1000);
  }
}

void fullStep() {
  PORTB = 0b00010000;
  delay(1);
  PORTB = 0b00100000;
  delay(1);
  PORTB = 0b01000000;
  delay(1);
  PORTB = 0b10000000;
  delay(1);
}

void halfStep() {
  PORTB = 0b00010000;
  delay(1);
  PORTB = 0b00110000;
  delay(1);
  PORTB = 0b00100000;
  delay(1);
  PORTB = 0b01100000;
  delay(1);
  PORTB = 0b01000000;
  delay(1);
  PORTB = 0b11000000;
  delay(1);
  PORTB = 0b10000000;
  delay(1);
  PORTB = 0b10100000;
  delay(1);
}

void main(void) {
  TRISB = 0b00001111; // Set PORTB pins 4-7 as outputs
  PORTB = 0b00000000; // Set all PORTB pins as low
  OSCCON = 0x70; // Set the internal oscillator frequency to 8 MHz

  while(1) {
    if(RB1 == 1) { // Check if the EN pin is high
      if(RB3 == 1) { // Check if the MODE pin is high
        if(RB2 == 1) { // Check if the DIR pin is high
          for(int i = 0; i < 100; i++) {
            fullStep();
          }
        } else if (RB2 == 0) { // DIR pin is low
          for(int i = 0; i < 100; i++) {
            fullStep();
          }
        }
      } else { // MODE pin is low
        if(RB2 == 1) { // Check if the DIR pin is high
          for(int i = 0; i < 100; i++) {
            halfStep();
          }
        } else { // DIR pin is low
          for(int i = 0; i < 100; i++) {
            halfStep();
          }
        }
      }
    } else { // EN pin is low
      PORTB = 0b00000000; // Set all PORTB pins as low
    }
  }
}