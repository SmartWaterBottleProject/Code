//Smart Water Bottle Project Main Function
//Programmers: Dean Pickett, Ryan Koons, and Matthew Woodruff
//UCF SD
//8-23-21

//**************   I changed the target configuration to 6989 for developing on the launchpad  ************************\\

#include <stdint.h>
#include <Ports.h>
#include <Initializer.h>
#include <UART.h>
#include <Batteryreader.h>
#include <Sanitizer.h>
#include <Analyzer.h>
#include <Exporter.h>


//Could use bool to save program space
bool StartSanitize = 0,StartAnalyze = 0, REED = 1; // Global variables to trigger sanitization, analyzing, and Reed safety shutoff
uint8_t BatteryLife = 100;  //uint8_t to save program space


int main (void)
{
    //Call Initializer.c, set pin-configuration
    initialize();

    //Call Batteryread
    BatteryLife = Batteryread();


    // Enable the global interrupt bit and enable low power mode
    _enable_interrupts();
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
            Sanitize(&REED);

            }
            StartSanitize = 0; // reset variable for calling sanitizer

            REED=1; //for testing Reed interrupt with analyzer button

            GPIO_enableInterrupt(SanitizeButtonPort, SanitizeButtonPin); //enable sanitize button interrupt
            GPIO_enableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //enable analyze button interrupt
        }

//Remove "Analyze()" in line below => Not sure why, that's just calling the analyze function -Dean
        if(StartAnalyze) //checks if analyze button was pressed
        {
            //Call Batteryread
           BatteryLife = Batteryread();

            if(BatteryLife >= 20) //Ensure there is enough battery life, prior to starting sanitization
            {
                //Call Analyzer.c
                Analyze();

            }
            StartAnalyze = 0; //reset variable for calling analyzer

            GPIO_enableInterrupt(SanitizeButtonPort, SanitizeButtonPin); //enable sanitize button interrupt
            GPIO_enableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //enable analyze button interrupt
        }

        //enable LPM again just in case
       _low_power_mode_4();

    }

    return 0;
}





//**********ISR********//
//*********************//

//Need interrupt for holding down sanitize button, disables sanitization mode and device returns to sleep
//why do we want to turn off with button when it shuts off when cap is unscrewed? A half turn of the lid
//would be enough to disengage the Reed switch

#pragma vector = PORT1_VECTOR  //Link the ISR to the Vector
__interrupt void P1_ISR()
{
   if(P1IFG & SanitizeButtonPin) // Detect button 1
   {
      StartSanitize = 1; // set global sanitize variable

       //Disable LPM
       LPM4_EXIT;


       //Deactivate both buttons
       GPIO_disableInterrupt(SanitizeButtonPort, SanitizeButtonPin);
       //GPIO_disableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);


       //P9OUT ^= (RedLED|GreenLED|YellowLED|BlueLED);  //Toggle LEDs, haven't messed with this yet, not
                                                       // sure why this is here

       GPIO_clearInterrupt(SanitizeButtonPort, SanitizeButtonPin);  //clear sanitize button interrupt
   }

   else if(P1IFG & AnalyzeButtonPin)
   {

       //StartAnalyze = 1; // set global analyze variable

       REED = 0; // this is for testing Reed interrupt functionality on launchpad
       //Disable LPM
       //LPM4_EXIT;


       //Deactivate both buttons
      // GPIO_disableInterrupt(SanitizeButtonPort, SanitizeButtonPin);
      // GPIO_disableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);


       //P9OUT ^= (RedLED|GreenLED|YellowLED|BlueLED);  //Toggle LEDs, haven't messed with this yet, not
                                                       // sure why this is here
       GPIO_clearInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //clear analyze button interrupt
   }

}


