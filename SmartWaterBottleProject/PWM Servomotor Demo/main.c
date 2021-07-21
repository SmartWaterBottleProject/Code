//Smart Water Bottle Project: Servomotor Demo
//Programmer: Ryan Koons
//UCF SD1
//7-19-21

//Task: Generate PWM output signal in order to re-position the servomotor.
//Period: 20 ms
// For 90 degree angle: ON pulse: 1500 microseconds
//For 100 degree angle: ON pulse: 1560 microseconds
    //Increment PWM duty cycle in 10 microsecond intervals
        //Use left button to decrease 10 microseconds
        //Use right button to increase 10 microseconds

#include <msp430.h>
//#include "driverlib.h"
#define Button1 BIT1     //P1.1
#define Button2 BIT2     //P1.2
#define PWMServo BIT6    //P1.6

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       //clear lock and allow new code re-flash/enable GPIO Pins



    //Configure P1.6 as TA0.1 for PWM
    P1DIR |= PWMServo;
    P1SEL1 |= PWMServo;
    P1SEL0 |= PWMServo;

    TA0CCR0=20000-1; //@1 Mhz, 20 millisecond
    TA0CTL= TASSEL_2 | ID_0 | MC_1 | TACLR;
    //Channel Reset/Set

    TA0CCTL1 |= OUTMOD_7;
    TA0CCR1=1500-1;


    /*
    // Configure P1.4 as TA1.0 for PWM
       //P1DIR.4=1, PSEL1.4=1, P1SEL0.4=1, LCDSz=0
       P1DIR |= BIT4; //SetP1DIR bit 4 as high
       P1SEL1 |= BIT4; //SetP1SEL1 bit 4 as high
       P1SEL0 |= BIT4; //Set P1SEL0 bit 4 as high
       LCDCCTL0 &= 0;  //Set output LED segments to 0

    //Configure PWM with 20ms period, and 1.5ms duty cycle
    //TACCR0 count for on time
    //TACCR1 count for total time
    TA1CCR1 = 20000-1; //@1Mhz, T=20 ms, (f=50 Hz)


    //Configure clock with SMCLK=1Mhz, No input division, Up mode, and clear TAR
    TA1CTL = TASSEL_2 | ID_0 | MC_1 | TACLR;
    TA1CCTL0 |= OUTMOD_7; // Set/Reset Output Pattern
    TA1CCR0=1500-1; //@1MHz, T= 1.5 ms (90 degree angle)*/






    //configuring buttons
    P1DIR &= ~(Button1|Button2); //0: input
    P1REN |= (Button1|Button2);  //1: enable built in resistors
    P1OUT |= (Button1|Button2);  //1: Pull up to Vcc
    P1IE |= (Button1|Button2);   //1:enable interrupts
    P1IES |= (Button1|Button2);  //1: interrupt on falling edge
    P1IFG &= ~(Button1|Button2); //0: Clear interrupt flags

    _enable_interrupts(); //Set global interrupt enable bit

while(1){}

    return 0;
}



//**********ISRs********//
//*********************//

#pragma vector = PORT1_VECTOR  //Link the ISR to the Vector
__interrupt void P1_ISR()
{
   if( (P1IFG&Button1) !=0 ) {
       //decrement duty cycle by 10 microseconds
       if(TA0CCR1 > (1500-1)  )
           TA0CCR1-=10;
   P1IFG &= ~Button1; //clear flag
   }

   if( (P1IFG&Button2)!= 0 ) {
       //increment duty cycle by 10 microseconds
       if(TA0CCR1 < (1560-1)  )
           TA0CCR1+=10;
   P1IFG &= ~Button2; //clear flag
   }

}
