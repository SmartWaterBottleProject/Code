//Smart Water Bottle Project Main Function
//Programmers: Dean Pickett, Ryan Koons, and Matthew Woodruff
//UCF SD
//8-23-21

//Current implementaton: Blue LED and Yellow LED--Sanitization. Green LED-enterring reed(); (cap removed)

//**************   I changed the target configuration to 6989 for developing on the launchpad  ************************\\

//Added bool StartOrStop--probably not needed and can be removed

#include <stdint.h>
#include <stdbool.h>
#include <Initializer.h>
#include <UART.h>
#include <Batteryreader.h>
#include <Sanitizer.h>
#include <Analyzer.h>
#include <Def.h>
#include <Exporter.h>


//Could use bool to save program space
bool StartSanitize = 0, StopSanitize =0, StartAnalyze = 0, REED = 1; // Global variables to trigger sanitization, analyzing, and Reed safety shutoff
bool ProcessRunningNot = 1;  //If process is currently running, do not run another one, 0-process is running, 1-process is NOT running
uint8_t BatteryLife = 100;  //uint8_t to save program space


int main (void)
{
    //Call Initializer.c, set pin-configuration
    initialize();

    //Call Batteryread
    BatteryLife = Batteryread();

    //See if cap is on or not
    if(GPIO_getInputPinValue(ReedSwitchPort, ReedSwitchPin) == 0)
    {
        //Cap is removed
        REED = 0;  //Reed switch is closed (cap removed)
        ProcessRunningNot =0;  //Process is running (reed switch removed)
        GPIO_clearInterrupt(ReedSwitchPort, ReedSwitchPin);  //Clear interrupt called for falling edge of reed switch
    }

    //Define main variables
//bool SanitizerStartOrStop=0;  //Used for declaring beginning or end of Process


    // Enable the global interrupt bit and enable low power mode
    _enable_interrupts();
//    sleep:
//    _low_power_mode_4();

    for(;;)
    {
        //when reed switch is closed (cap removed) poll for cap being reattached
        if((REED == 0) && (ProcessRunningNot == 0))
        {
            reed();  //Call reed polling function
        }
        if(StartSanitize && ProcessRunningNot) //checks if sanitize button was pressed, and no other process is running
        {
            //Call Batteryread
            BatteryLife = Batteryread();

            if(BatteryLife>= 20)  //Ensure there is enough battery life, prior to starting sanitization
            {
            //Call Sanitizer.c to start sanitization
            ProcessRunningNot =0;  //Blocking term for process
            Sanitize(&REED, START);  //Call sanitization function to start sanitizer
//            StopSanitize =1;  //Next time this if statement executes, the sanitizer should stop--might not need this

            }

            else {
                //Should eventually Blink red LED for low battery
                GPIO_setOutputLowOnPin(RedLEDNOTPort, RedLEDNOTPin);  //Turn Red LED ON
            }

            StartSanitize = 0;  //Clear sanitization mode, not enough battery life to perform

//            StartSanitize = 0; // reset variable for calling sanitizer, --I dont know if we want to do this yet RKK

//            REED=1; //for testing Reed interrupt with analyzer button
//
//            GPIO_enableInterrupt(SanitizeButtonPort, SanitizeButtonPin); //enable sanitize button interrupt
//            GPIO_enableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //enable analyze button interrupt
        }

//RKK Dont think we need this actually
//        if(StopSanitize)  //Terminate the UVC LED and process
//        {
//            Sanitize(&REED, STOP);  //Call sanitize.c to stop sanitizer
//            StopSanitize=0;  //Sanitization is finished
//        }

//Remove "Analyze()" in line below => Not sure why, that's just calling the analyze function -Dean
        if(StartAnalyze && ProcessRunningNot) //checks if analyze button was pressed, and no other process is running
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

        //enable LPM again just in case--I think this is actually really bad
      _low_power_mode_4();

    }

    return 0;
}





//**********ISR********//
//*********************//

//Need interrupt for holding down sanitize button, disables sanitization mode and device returns to sleep
//why do we want to turn off with button when it shuts off when cap is unscrewed? A half turn of the lid
//would be enough to disengage the Reed switch--RKK Not sure I follow

//Need to add interrupt for Reed switch transitioning low (cap being removed from bottle) P2.7 LLWU Interrupt Vector
#pragma vector = PORT2_VECTOR  //Link the ISR to the Vector
__interrupt void P2_ISR()
{
    if(P2IFG & ReedSwitchPin)  //Detect Reed Switch closing
    {
        //Stop Everything
        GPIO_setOutputLowOnPin(UVCEnablePort, UVCEnablePin);        //Timer disables UVC LEDs
        GPIO_setOutputHighOnPin(YellowLEDNOTPort, YellowLEDNOTPin); //Timer turns off yellow indicator LED
        REED = 0;
        ProcessRunningNot =0;  //Process is running (reed switch removed)
        LPM4_EXIT;
        GPIO_clearInterrupt(ReedSwitchPort, ReedSwitchPin);  //Clear interrupt called for falling edge of reed switch

    }
}

//Need to add interrupt for 3 second timer when button is held down during device mode

#pragma vector = PORT1_VECTOR  //Link the ISR to the Vector
__interrupt void P1_ISR()
{
   if(P1IFG & SanitizeButtonPin) // Detect button 1
   {
       //Check if sanitizer is already running, when button is held for three seconds during sanitization, UVCs turn off
//       if( StartSanitize)
//       {
//           //Set timer for 3 seconds to see if button is still held
//       }
//       else
//       {
      StartSanitize = 1; // set global sanitize variable

       //Disable LPM
       LPM4_EXIT;


       //Deactivate both buttons
       GPIO_disableInterrupt(SanitizeButtonPort, SanitizeButtonPin);
       GPIO_disableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);


       //P9OUT ^= (RedLED|GreenLED|YellowLED|BlueLED);  //Toggle LEDs, haven't messed with this yet, not
                                                       // sure why this is here

       GPIO_clearInterrupt(SanitizeButtonPort, SanitizeButtonPin);  //clear sanitize button interrupt
  // }

   }

   else if(P1IFG & AnalyzeButtonPin)
   {

       //StartAnalyze = 1; // set global analyze variable

      // REED = 0; // this is for testing Reed interrupt functionality on launchpad
       //Disable LPM
       //LPM4_EXIT;

       LPM4_EXIT;  //I think this is necessary?
       //Deactivate both buttons
      // GPIO_disableInterrupt(SanitizeButtonPort, SanitizeButtonPin);
      // GPIO_disableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);


       //P9OUT ^= (RedLED|GreenLED|YellowLED|BlueLED);  //Toggle LEDs, haven't messed with this yet, not
                                                       // sure why this is here
       GPIO_clearInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //clear analyze button interrupt
   }

}
//When sanitizer is done
#pragma vector = TIMER0_A0_VECTOR // Link the ISR to the vector
__interrupt void T0A0_ISR() {

//    sanitize = 0; //end sanitize function
    GPIO_setOutputLowOnPin(UVCEnablePort, UVCEnablePin);        //Timer disables UVC LEDs
    GPIO_setOutputHighOnPin(YellowLEDNOTPort, YellowLEDNOTPin); //Timer turns off yellow indicator LED
//    GPIO_setOutputLowOnPin(GreenLEDNOTPort, GreenLEDNOTPin);  //This is only here to make sure this vector triggers in main and terminates UVCs
    //Clear Timer A0 flag and disable interrupt
    TA0CCTL0 &= ~CCIE; // Disable Channel 0 CCIE bit
    TA0CCTL0 &= ~CCIFG; // Clear Channel 0 CCIFG bit


    ProcessRunningNot = 1; //Process is no longer running

    GPIO_enableInterrupt(SanitizeButtonPort, SanitizeButtonPin); //enable sanitize button interrupt
    GPIO_enableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //enable analyze button interrupt

    // Hardware clears the flag, CCIFG in TA0CCTL0
}





