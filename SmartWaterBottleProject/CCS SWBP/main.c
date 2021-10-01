//Smart Water Bottle Project Main Function
//Programmers: Dean Pickett, Ryan Koons, and Matthew Woodruff
//UCF SD
//8-23-21

#include "driverlib.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <Ports.h>
#include <Initializer.h>
#include <UART.h>
#include <Batteryreader.h>
#include <Sanitizer.h>
#include <Analyzer.h>
#include <Exporter.h>

//I don't think you need function prototypes for vectors
//__interrupt void  Port1_ISR() ;

//Could use bool to save program space
bool StartSanitize = 0,StartAnalyze = 0; // Global variables to trigger sanitization and analyzing
uint8_t BatteryLife = 100;  //uint8_t to save program space


int main (void)
{
    //Call Initializer.c, set pin-configuration
    initialize();

    //Call Batteryread
    BatteryLife = Batteryread();

    //Enter LPM, also enable GIE bit (don't call until interrupts are set-up)
    _low_power_mode_4();

    while(1)
    {
        if(StartSanitize) //checks if sanitize button was pressed
        {
            //Call Batteryread
            BatteryLife = Batteryread();

            if(BatteryLife>= 20)  //Ensure there is enough battery life, prior to starting sanitization
            {
            //Call Sanitizer.c
            Sanitize();
            //Reactivate all button presses
            }
            StartSanitize = 0; //reset variable for calling analyzer
        }

//Remove "Analyze()" in line below
        if(StartAnalyze) //checks if analyze button was pressed
        {
            //Call Batteryread
           BatteryLife = Batteryread();

            if(BatteryLife >= 20) //Ensure there is enough battery life, prior to starting sanitization
            {
                //Call Analyzer.c
                Analyze();
               //Reactivate all button presses
            }
            StartAnalyze = 0; //reset variable for calling analyzer
        }

        //enable LPM again just in case
       _low_power_mode_4();

    }

    return 0;
}





//**********ISR********//
//*********************//

//Need interrupt for holding down sanitize button, disables sanitization mode and device returns to sleep

#pragma vector = PORT1_VECTOR  //Link the ISR to the Vector
__interrupt void P1_ISR()
{
   if(P1IFG & sanitizebutton) // Detect button 1
   {
      StartSanitize = 1; // set global sanitize variable

       //Disable LPM
       LPM4_EXIT;

       //Disable other button?
       //Deactivate current button?


       //P9OUT ^= (RedLED|GreenLED|YellowLED|BlueLED);  //Toggle LEDs
       P1IFG &= ~sanitizebutton; //clear flag (shared)
   }

   if( (P1IFG & waterqualitybutton)!= 0 )
   {

       StartAnalyze = 1; // set global analyze variable

       //Disable LPM
       LPM4_EXIT;


       //Disable other button?
       //Deactivate current button?


       //P9OUT ^= (RedLED|GreenLED|YellowLED|BlueLED);  //Toggle LEDs
       P1IFG &= ~waterqualitybutton; //clear flag (shared)
   }

}
