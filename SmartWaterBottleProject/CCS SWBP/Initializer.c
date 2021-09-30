/*
 * Initialize.c
 *
 *  Created on: Aug 23, 2021
 *      Author: User
 */

//Configure GPIO Pins
    //Setting up ADC reads
    //Initialize UART
//Set-up all of the timer modules

#include <Ports.h>
#include <Initializer.h>

void initialize(void) {
       WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
       PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

      // P9DIR |= (redLED|greenLED|yellowLED|blueLED);    //direct port 1 pins as output
      // P9OUT |= (redLED|greenLED|yellowLED|blueLED);    //turn LEDs OFF (active low)
       // THESE 4 LINES ARE FOR THE LAUNCHPAD
           P1DIR |= redLED; // Direct pin as output
           P9DIR |= greenLED; // Direct pin as output
           P1OUT &= ~redLED; // Turn LED Off
           P9OUT &= ~greenLED; // Turn LED Off


       //configuring buttons
       P1DIR &= ~(sanitizebutton |waterqualitybutton); //0: input
       P1REN |= (sanitizebutton |waterqualitybutton);  //1: enable built in resistors
       P1OUT |= (sanitizebutton |waterqualitybutton);  //1: Pull up to Vcc
       P1IE |= (sanitizebutton |waterqualitybutton);   //1:enable interrupts
       P1IES |= (sanitizebutton |waterqualitybutton);  //1: interrupt on falling edge
       P1IFG &= ~(sanitizebutton |waterqualitybutton); //0: Clear interrupt flags

       // Enable the global interrupt bit (call an intrinsic function)
       _enable_interrupts();


    return;
}

