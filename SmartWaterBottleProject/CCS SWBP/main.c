//Smart Water Bottle Project Main Function
//Programmers: Ryan Koons, Matthew Woodruff, and Dean Pickett
//UCF SD1
//7-19-21

//Code uses S1 and S2 to toggle water bottle LED indicators ON/OFF

#include <msp430.h>
#define RedLED BIT0    //red LED at P9.0
#define GreenLED BIT1  //green LED at  P9.7
#define YellowLED BIT5 //Yellow LED at P9.5
#define BlueLED BIT6
#define SanitizeButton BIT1     //P1.1
#define WaterQualityButton BIT2 //P1.2

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       //clear lock and allow new code re-flash/enable GPIO Pins

    P9DIR |= (RedLED|GreenLED|YellowLED|BlueLED);    //direct pin as output
    P9OUT |= (RedLED|GreenLED|YellowLED|BlueLED);    //turn LEDs OFF (active low)

    //configuring buttons
    P1DIR &= ~(SanitizeButton|WaterQualityButton); //0: input
    P1REN |= (SanitizeButton|WaterQualityButton);  //1: enable built in resistors
    P1OUT |= (SanitizeButton|WaterQualityButton);  //1: Pull up to Vcc
    P1IE |= (SanitizeButton|WaterQualityButton);   //1:enable interrupts
    P1IES |= (SanitizeButton|WaterQualityButton);  //1: interrupt on falling edge
    P1IFG &= ~(SanitizeButton|WaterQualityButton); //0: Clear interrupt flags

    //lastly enable GIE, avoid raising interrupts too soon. Enter low power mode
    _low_power_mode_4();



    return 0;
}

//**********ISR********//
//*********************//
#pragma vector = PORT1_VECTOR  //Link the ISR to the Vector
__interrupt void P1_ISR()
{
   if( (P1IFG&SanitizeButton) !=0 ) {
       P9OUT ^= (RedLED|GreenLED|YellowLED|BlueLED);  //Toggle LEDs
   P1IFG &= ~SanitizeButton; //clear flag (shared)
   }

   if( (P1IFG&WaterQualityButton)!= 0 ) {
       P9OUT ^= (RedLED|GreenLED|YellowLED|BlueLED);  //Toggle LEDs
   P1IFG &= ~WaterQualityButton; //clear flag (shared)
   }

}
