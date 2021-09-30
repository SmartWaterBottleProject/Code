//Smart Water Bottle Project Main Function
//Programmers: Ryan Koons, Matthew Woodruff, and Dean Pickett
//UCF SD
//8-23-21


#include <Ports.h>
#include <Initializer.h>
#include <UART.h>
#include <Batteryreader.h>
#include <Sanitizer.h>
#include <Analyzer.h>
#include <Exporter.h>
__interrupt void  Port1_ISR() ;

int sanit = 0, anaz = 0; // Global variables to trigger sanitization and analyzing
int battcharge = 100;


int main (void)
{
    //Call Initializer.c
    initialize();

    //Call Batteryread
    battcharge = Batteryread();

    //Enter LPM
    _low_power_mode_4();

    while(1)
    {
        if(sanit) //checks if sanitize button was pressed
        {
            //Call Batteryread
            battcharge = Batteryread();

            if(battcharge >= 20){
            //Call Sanitizer.c
            Sanitize();

            //Reactivate all button presses
            }
            sanit = 0; //reset variable for calling analyzer
        }

        if(anaz) //checks if analyze button was pressed
        {
            //Call Batteryread
            battcharge = Batteryread();

            if(battcharge >= 20){
                //Call Analyzer.c
                Analyze();

               //Reactivate all button presses
            }
            anaz = 0; //reset variable for calling analyzer
        }

        //enable LPM again just in case
       _low_power_mode_4();

    }

    return 0;
}





//**********ISR********//
//*********************//

#pragma vector = PORT1_VECTOR  //Link the ISR to the Vector
__interrupt void P1_ISR()
{
   if(P1IFG & sanitizebutton) // Detect button 1
   {
       sanit = 1; // set global sanitize variable

       //Disable LPM
       LPM4_EXIT;

       //Disable other button?
       //Deactivate current button?


       //P9OUT ^= (RedLED|GreenLED|YellowLED|BlueLED);  //Toggle LEDs
       P1IFG &= ~sanitizebutton; //clear flag (shared)
   }

   if( (P1IFG & waterqualitybutton)!= 0 )
   {

       anaz = 1; // set global analyze variable

       //Disable LPM
       LPM4_EXIT;


       //Disable other button?
       //Deactivate current button?


       //P9OUT ^= (RedLED|GreenLED|YellowLED|BlueLED);  //Toggle LEDs
       P1IFG &= ~waterqualitybutton; //clear flag (shared)
   }

}
