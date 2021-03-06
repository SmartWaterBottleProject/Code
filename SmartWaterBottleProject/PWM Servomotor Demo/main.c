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
#define Button1 BIT1     //P1.1
#define Button2 BIT2     //P1.2
#define PWMServo BIT6    //P1.6
#define LimLow 1280   //Lower Limit
#define LimHi  1680   //Upper Limit
#define Start  1480   //Starting Point
#define Step   10       //Step Size

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
    TA0CCR1 = Start; //(LimLow+LimHi)>>1; //Gets to the middle value.

    //configuring buttons
    P1DIR &= ~(Button1|Button2); //0: input
    P1REN |= (Button1|Button2);  //1: enable built in resistors
    P1OUT |= (Button1|Button2);  //1: Pull up to Vcc
    P1IE |= (Button1|Button2);   //1:enable interrupts
    P1IES |= (Button1|Button2);  //1: interrupt on falling edge
    P1IFG &= ~(Button1|Button2); //0: Clear interrupt flags

    _low_power_mode_0(); //enable interrupts, enter LPM0

    return 0;
}



//**********ISRs********//
//*********************//

#pragma vector = PORT1_VECTOR  //Link the ISR to the Vector
__interrupt void P1_ISR()
{
    //If Button1 was pressed
   if( (P1IFG&Button1) !=0 )
   {
      if(TA0CCR1 > LimLow)
           TA0CCR1 -= Step;
   P1IFG &= ~Button1; //clear flag
   }

   //If Button2 was pressed
   if( (P1IFG&Button2)!= 0 )
   {
       if(TA0CCR1 < LimHi)
           TA0CCR1 += Step; //increment duty cycle by 10 microseconds
   P1IFG &= ~Button2; //clear flag
   }
}
