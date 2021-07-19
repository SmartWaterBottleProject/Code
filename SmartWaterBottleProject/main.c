//4.1--Buttons with Interrupts
#include <msp430.h> 
#define redLED BIT0   //red LED at P1.0
#define greenLED BIT7 //green LED at  P9.7
#define BUT1 BIT1 //P1.1
#define BUT2 BIT2 //P1.2

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       //clear lock and allow new code re-flash/enable GPIO Pins

    P1DIR |= redLED;    //direct pin as output
    P9DIR |= greenLED;  //direct pin as output
    P1OUT &= ~redLED;  //turn LED OFF
    P9OUT &= ~greenLED;//turn LED OFF

    //configuring buttons
    P1DIR &= ~(BUT1|BUT2); //0: input
    P1REN |= (BUT1|BUT2);  //1: enable built in resistors
    P1OUT |= (BUT1|BUT2);  //1: Pull up to Vcc
    P1IE |= (BUT1|BUT2);   //1:enable interrupts
    P1IES |= (BUT1|BUT2);  //1: interrupt on falling edge
    P1IFG &= ~(BUT1|BUT2); //0: Clear interrupt flags

    //lastly enable GIE, avoid raising interrupts too soon
    _enable_interrupts();

    for(;;){

    }

    return 0;
}

//**********ISR********//
//*********************//
#pragma vector = PORT1_VECTOR  //Link the ISR to the Vector
__interrupt void P1_ISR()
{
   if( (P1IFG&BUT1) !=0 ) {
       P1OUT ^= redLED;
   P1IFG &= ~BUT1; //clear flag (shared)
   }

   if( (P1IFG&BUT2)!= 0 ) {
       P9OUT ^= greenLED;
   P1IFG &= ~BUT2; //clear flag (shared)
   }

}
