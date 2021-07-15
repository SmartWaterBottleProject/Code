// Code that flashes the red LED
#include <msp430.h> //header file for micro chip
#define redLED BIT0 // Red LED at P1.0

//start main function
void main(void)
{
volatile unsigned int i;  //counting loop variable
WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
PM5CTL0 &= ~LOCKLPM5; // Disable GPIO power-on default high impedance mode

P1DIR |= redLED; // Direct pin as output
P1OUT &= ~redLED; // Turn LED Off (clear bit, rest unchanged)

for(;;)
    {
    for(i=0; i<90000; i++) {} //delay loop
    P1OUT ^= redLED; // Toggle the LED on then off
    }
}
