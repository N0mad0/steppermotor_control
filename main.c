/*
* Project Title: Unipolar Stepper Motor Control Assignment Version 4
* Author: Dylan Pearse: K00255671
* File Created on: 06/02/2023 10:30 
* File Last Modified on: 08/08/2023 15:30
* Stepper Motor Controller Operational Specification:-
1. The Controller must drive a 6-wire, 12V Unipolar Stepper Motor using a PIC16F88.
2. The Controller has 4 control/input pins (STEP, EN, DIR, MODE) which define its operation:
    STEP: (RB0)
        A rising edge on the STEP pin is used to advance the stepper motor by either a FullStep or Half-Step depending on if the MODE pin is logic high or low respectively and
        the EN pin is high.
    EN: (RB1)
        When the EN pin is high the Stepper Motor is enabled to be driven in either Full-Step
        or Half-Step mode via the MODE pin. If the EN pin is low, then all 4 output pin (RB4-
        RB7) controlling the stepper motor must be driven low (i.e. off).
    DIR: (RB2)
        When the Dir pin is high, the Stepper Motor rotates clockwise.
        When the Dir pin is low, the Stepper Motor rotates counter-clockwise.
    MODE: (RB3)
        When the MODE pin is high the Stepper Motor is driven in Full-Step mode.
        When the MODE pin is low the Stepper Motor is driven in Half-Step mode.    
3. The Controller has 4 output pins (RB4,RB5,RB6,RB7) which are used to drive the gates of 4 N-channel Mosfets which are used to drive the 4 coils of the Unipolar stepper motor.
4. This board uses the internal oscillator of the PIC16F88. To setup the internal oscillator do the following:-
    (i) Ensure to set the Configuration Bit ? FOSC = INTIOSCIO
    (ii) Inside main(), set OSCCON = 0x70; // this sets Internal Oscillator
    Frequency @8MHz (see page 40 of PIC16F88 Datasheet). 
*/

// PIC16F88 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1
#pragma config FOSC = INTOSCIO          // Oscillator Selection bits (Internal oscillator)
#pragma config WDTE = OFF               // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF              // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON               // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF              // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF                // Low-Voltage Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF                // Data EE Memory Code Protection bit (Code protection off)
#pragma config WRT = OFF                // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CCPMX = RB0              // CCP1 Pin Selection bit (CCP1 function on RB0)
#pragma config CP = OFF                 // Flash Program Memory Code Protection bit (Code protection off)

// CONFIG2
#pragma config FCMEN = OFF              // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF               // Internal External Switchover bit (Internal External Switchover mode disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#define STEP RB0                        // Defining RB0 as STEP for easier Reference
#define EN RB1                          // Defining RB1 as EN(Enable) for easier Reference
#define DIR RB2                         // Defining RB2 as DIR(Direction) for easier Reference
#define MODE RB3                        // Defining RB3 as MODE for easier Reference
#define ON 1                            // Defining Logic Value 1 as ON for easier Reference
#define OFF 0                           // Defining Logic Value 0 as OFF for easier Reference
unsigned char count;                    // Defining Counter 

void fullStep (unsigned char count);    // Function Full Step Declaration 
void halfStep (unsigned char count);    // Function Half Step Declaration

void main(void) {
  TRISB = 0b00001111;                   // Set PORTB pins 4-7 as outputs
  ANSEL = 0;                            // Set ANSEL as OFF
  PORTB = 0b00000000;                   // Set all PORTB pins as low
  OSCCON = 0x70;                        // Set the internal oscillator frequency to 8 MHz
  TRISA = 0xFF;                         // Configure all Inputs
  count = 0;                            // Set count to 0

  while(1)                              // Loop Forever 
    {
      while(EN && !MODE)                // While EN is High and Mode is Low Enter Fullstep mode
      {
      while(STEP == OFF);
          if(DIR == OFF)                // If DIR is Low turn clockwise
          {
              count++;                  // Increase Count by 1
              if (count == 5)           // If counter is equal to 5
              {
                  count = 1;            // Reset Counter
              }
          }
          else if (DIR == ON)           // If DIR is High turn counter-clockwise           
          {
              count --;                 // Decrease count by 1
              if (count == 0)           // If count is zero enter IF Statement and reset count to 4
              {
                  count = 4;
              }   
          }
          fullStep(count);              // Call of Full Step Function
	  while(STEP == ON);                // While STEP is High allow to reset loop
      }
      while(EN && MODE)                 // While EN and MODE are High enable Half Step mode         
      {
      while(STEP == OFF);               // If Step Button is Low enter Loop
          if(DIR == OFF)                // If Direction is low turn clockwise
          {
              count++;
              if (count == 9)           // If count is 9 set count to 1
                {
                  count = 1;
                }
          }
          else if (DIR == ON)           // If DIR is high turn anti clockwise
          {
              count --;                 // subtract count
              if (count == 0)           // if count is reset to 8
                {
                  count = 8;
                }   
          }
          halfStep(count); 
	  while(STEP == ON);
        }
    }
}
    
void fullStep(unsigned char count)
{
    switch(count)
    {
        case 1:
            RB4 = 1;
            RB5 = 0;
            RB6 = 1;
            RB7 = 0;
            break;
	    
        case 2:
            RB4 = 0;
            RB5 = 1;
            RB6 = 1;
            RB7 = 0;
            break;
	    
        case 3:            
            RB4 = 0;
            RB5 = 1;
            RB6 = 0;
            RB7 = 1;
            break;
	    
        case 4:    
            RB4 = 1;
            RB5 = 0;
            RB6 = 0;
            RB7 = 1;    
            break;
        
            default:
            RB4 = 0;
            RB5 = 0;
            RB6 = 0;
            RB7 = 0;       
    }
}

void halfStep(unsigned char count)
{
    switch(count)
    {
        case 1:
            RB4 = 1;
            RB5 = 0;
            RB6 = 0;
            RB7 = 0;
            break;
            
        case 2:
            RB4 = 1;
            RB5 = 0;
            RB6 = 1;
            RB7 = 0;
            break;
	    
        case 3:
            RB4 = 0;
            RB5 = 0;
            RB6 = 1;
            RB7 = 0;
            break;
	    
        case 4:
            RB4 = 0;
            RB5 = 1;
            RB6 = 1;
            RB7 = 0;
            break;
	    
	    case 5:
            RB4 = 0;
            RB5 = 1;
            RB6 = 0;
            RB7 = 0;
            break;
	    
        case 6:
            RB4 = 0;
            RB5 = 1;
            RB6 = 0;
            RB7 = 1;
            break;
	    
        case 7:
            RB4 = 0;
            RB5 = 0;
            RB6 = 0;
            RB7 = 1;     
            break;
	   
        case 8:
            RB4 = 1;
            RB5 = 0;
            RB6 = 0;
            RB7 = 1;
            break;
            
        default:
            RB4 = 0;
            RB5 = 0;
            RB6 = 0;
            RB7 = 0;     
    }
}