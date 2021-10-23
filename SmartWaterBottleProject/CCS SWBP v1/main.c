//Smart Water Bottle Project Main Function
//Programmers: Dean Pickett, Ryan Koons, and Matthew Woodruff
//UCF SD
//8-23-21

//Current implementation: Blue LED and Yellow LED--Sanitization. Green LED-enterring reed(); (cap removed)

//**************   I changed the target configuration to 6989 for developing on the launchpad  ************************\\

//Added bool StartOrStop--probably not needed and can be removed

#include <msp430.h>
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
bool StartSanitize = 0, StopSanitize =0, StartAnalyze = 0, ReedOpen = 1, UVCCheck =0 ; // Global variables to trigger sanitization, analyzing, and Reed safety shutoff
//ReedOpen=1, cap is on ReedOpen=0 cap is off
bool ProcessRunningNot = 1;  //If process is currently running, do not run another one, 0-process is running, 1-process is NOT running
uint8_t BatteryLife = 100;  //uint8_t to save program space

void reed();  //Function for polling reed switch when cap is removed


int main (void)
{
    //Call Initializer.c, set pin-configuration
    initialize();

    //Call Batteryread
    BatteryLife = Batteryread();

    _enable_interrupts();

    //See if cap is on or not
    if(GPIO_getInputPinValue(ReedSwitchPort, ReedSwitchPin) == 0)
    {
        //Cap is removed
        ReedOpen = 0;  //Reed switch is closed (cap removed)
        ProcessRunningNot =0;  //Process is running (reed switch removed)
        GPIO_clearInterrupt(ReedSwitchPort, ReedSwitchPin);  //Clear interrupt called for falling edge of reed switch
    }

    //Define main variables
//bool SanitizerStartOrStop=0;  //Used for declaring beginning or end of Process


    // Enable the global interrupt bit and enable low power mode

//    sleep:
//    _low_power_mode_4();

    for(;;)
    {
        //when reed switch is closed (cap removed) poll for cap being reattached
        if((ReedOpen == 0) && (ProcessRunningNot == 0))
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
            Sanitize(&ReedOpen, START);  //Call sanitization function to start sanitizer
            UVCCheck = 1;  //Signify UVC Check during first 10 seconds
//            StopSanitize =1;  //Next time this if statement executes, the sanitizer should stop--might not need this

            }

            else
            {
                //Should eventually Blink red LED for low battery
                GPIO_setOutputLowOnPin(RedLEDNOTPort, RedLEDNOTPin);  //Turn Red LED ON
            }

           StartSanitize = 0;  //Clear sanitization mode, not enough battery life to perform

//            StartSanitize = 0; // reset variable for calling sanitizer, --I dont know if we want to do this yet RKK

//            ReedOpen=1; //for testing Reed interrupt with analyzer button
//
//            GPIO_enableInterrupt(SanitizeButtonPort, SanitizeButtonPin); //enable sanitize button interrupt
//            GPIO_enableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //enable analyze button interrupt
        }

//RKK Dont think we need this actually
//        if(StopSanitize)  //Terminate the UVC LED and process
//        {
//            Sanitize(&ReedOpen, STOP);  //Call sanitize.c to stop sanitizer
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
               // Analyze();

            }
//            StartAnalyze = 0; //reset variable for calling analyzer

            GPIO_enableInterrupt(SanitizeButtonPort, SanitizeButtonPin); //enable sanitize button interrupt
            GPIO_enableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //enable analyze button interrupt
        }

        //enable LPM again just in case
      _low_power_mode_4();

    }

    return 0;
}

//For launchpad: Use breadboard short/open between P2.7 and nearby ground pin to simulate reed switch behavior

//Once reed switch is removed, do the following:
    //Disable Pull up on reed signal to eliminate current draw
    //Enable pull down resistor on reed signal
//    Wait one second
//    Re-enable pullup and check to see if pin is high or low
//        if (high): Reed switch is open, and cap is reattached to device.
//                Keep pull up on pin and return to normal device operation.
//                ReedOpen=1
//                ProcessRunningNot = 1
//        if (low) Reed switch is stil closed (shorted to 0V), disable pull up, and re-enable pull down
void reed()
{
    //Check to see if cap was placed back on
    if(GPIO_getInputPinValue(ReedSwitchPort, ReedSwitchPin) == 1)
    {
 //DOnt need this
//        GPIO_setAsInputPinWithPullUpResistor(ReedSwitchPort, ReedSwitchPin);//Reconfigure reed switch pin as pulled high input
        ProcessRunningNot = 1;  //Process no longer running
        ReedOpen = 1 ; //Cap is back on, reed switch is open
        return;
    }

    else
    {
        //Disable interrupts for buttons
        GPIO_disableInterrupt(SanitizeButtonPort, SanitizeButtonPin);  //Disable interrupt for sanitization button
        GPIO_disableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);    //Disable interrupt for analysis button

        GPIO_setOutputLowOnPin(GreenLEDNOTPort, GreenLEDNOTPin);  //Turn on Green
        GPIO_setAsInputPinWithPullDownResistor(ReedSwitchPort, ReedSwitchPin);  //Switch pull up to pull down reed switch (this eliminates current draw)



//        //Configure Channel 0 for up mode with interrupt
        TA0CCR0= (625-1); // 1 second period @ 39kHz
        TA0CCTL0 |= CCIE;  //enable Channel 0 CCIE Bit
        TA0CCTL0 &= ~CCIFG; //clear channel 0 CCIFG Bit
//        //Configure Timer_A: ACLK @ 625 Hz, Divide by 1, up Mode, clear TAR, enable interrupt for rollback event every 1s (leaves TAIE=0)
        TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;

      _low_power_mode_4();

        if(ReedOpen)
        {

            //Process is no longer running
            ProcessRunningNot = 1;  //Process no longer running
            ReedOpen = 1 ; //Cap is back on, reed switch is open
            GPIO_setOutputHighOnPin(GreenLEDNOTPort, GreenLEDNOTPin);  //Turn Green LED Off

            //Re-enable button interupts
            GPIO_clearInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //clear analyze button interrupt
            GPIO_clearInterrupt(SanitizeButtonPort, SanitizeButtonPin);  //clear sanitization button interrupt
            GPIO_enableInterrupt(SanitizeButtonPort, SanitizeButtonPin);  //Enable interrupt for sanitization button
            GPIO_enableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);    //Enable interrupt for analysis button
            GPIO_setOutputHighOnPin(GreenLEDNOTPort, GreenLEDNOTPin);  //Turn green LED OFF
            return;
        }
    }
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
        ReedOpen = 0;
        ProcessRunningNot =0;  //Process is running (reed switch removed)
        LPM4_EXIT;
        GPIO_clearInterrupt(ReedSwitchPort, ReedSwitchPin);  //Clear interrupt called for falling edge of reed switch

    }
}

//Need to add interrupt for 3 second timer when button is held down during device mode

#pragma vector = PORT1_VECTOR  //Link the ISR to the Vector
__interrupt void P1_ISR()
{
    //Deactivate both buttons
    GPIO_disableInterrupt(SanitizeButtonPort, SanitizeButtonPin);
    GPIO_disableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);

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




       //P9OUT ^= (RedLED|GreenLED|YellowLED|BlueLED);  //Toggle LEDs, haven't messed with this yet, not
                                                       // sure why this is here

       GPIO_clearInterrupt(SanitizeButtonPort, SanitizeButtonPin);  //clear sanitize button interrupt
  // }

   }

   else if(P1IFG & AnalyzeButtonPin)
   {

       StartAnalyze = 1; // set global analyze variable

      // ReedOpen = 0;// this is for testing Reed interrupt functionality on launchpad
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

#pragma vector = TIMER0_A0_VECTOR // Link the ISR to the vector
__interrupt void T0A0_ISR() {

    if(ReedOpen == 0)
    {
           //Clear Timer A0 flag
           //TA0CCTL0 &= ~CCIE; // Disable Channel 0 CCIE bit
           TA0CCTL0 &= ~CCIFG; // Clear Channel 0 CCIFG bit
           GPIO_setAsInputPinWithPullUpResistor(ReedSwitchPort, ReedSwitchPin);//Reconfigure reed switch pin as pulled high input

           if(GPIO_getInputPinValue(ReedSwitchPort, ReedSwitchPin) == 1)     //Check to see if cap was placed back on
           {
              ReedOpen = 1; //Cap is no longer off, switch is open again
              LPM4_EXIT;  //Exit low power mode
           }
           else
           {
               GPIO_setAsInputPinWithPullDownResistor(ReedSwitchPort, ReedSwitchPin);//Reconfigure reed switch pin as pulled low input (eliminate current draw)
               GPIO_toggleOutputOnPin(GreenLEDNOTPort, GreenLEDNOTPin);  //Toggle Green LED every second
           }
    }

    else if (!UVCCheck)
    {
        //    sanitize = 0; //end sanitize function
            GPIO_setOutputLowOnPin(UVCEnablePort, UVCEnablePin);        //Timer disables UVC LEDs
            GPIO_setOutputHighOnPin(YellowLEDNOTPort, YellowLEDNOTPin); //Timer turns off yellow indicator LED
        //    GPIO_setOutputLowOnPin(GreenLEDNOTPort, GreenLEDNOTPin);  //This is only here to make sure this vector triggers in main and terminates UVCs
            //Clear Timer A0 flag and disable interrupt
            TA0CCTL0 &= ~CCIE; // Disable Channel 0 CCIE bit
            TA0CCTL0 &= ~CCIFG; // Clear Channel 0 CCIFG bit


            ProcessRunningNot = 1; //Process is no longer running
            GPIO_clearInterrupt(SanitizeButtonPort, SanitizeButtonPin);  //clear sanitize button interrupt
            GPIO_clearInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);
            GPIO_enableInterrupt(SanitizeButtonPort, SanitizeButtonPin); //enable sanitize button interrupt
            GPIO_enableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //enable analyze button interrupt

    }

    else
    {
        //UVCCheck
        GPIO_setOutputHighOnPin(PhotoresistorEnablePort, PhotoresistorEnablePin); //Enable photo resistor
        Initialize_ADC_Photoresistor();  //Initialize the ADC
        ADC12CTL0 |= ADC12SC; //set, start conversion for adc
        //wait for flag to clear
        while( (ADC12CTL1 & ADC12BUSY) != 0 ){} //wait here, use !=0, since there could be other bits in bit field
        uint16_t PhotoresistorVoltage = ADC12MEM0;
        ADC12CTL0 &= ~ADC12ON;  //Turn ADC off? Maybe leaving it on causes, current draw.
        if( PhotoresistorVoltage <= 2500)
        {
            GPIO_setOutputLowOnPin(UVCEnablePort, UVCEnablePin);  //Disable UVCs
            GPIO_setOutputHighOnPin(YellowLEDNOTPort, YellowLEDNOTPin);  //Turn off yellow indicator LED
            TA0CCTL0 &= ~CCIE; // Disable Channel 0 CCIE bit
            TA0CCTL0 &= ~CCIFG; // Clear Channel 0 CCIFG bit
            ProcessRunningNot = 1; //Process is no longer running
            UVCCheck =0;  //UVC Check is done
            GPIO_clearInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //clear analyze button interrupt
            GPIO_clearInterrupt(SanitizeButtonPort, SanitizeButtonPin);  //clear sanitization button interrupt
            GPIO_enableInterrupt(SanitizeButtonPort, SanitizeButtonPin); //enable sanitize button interrupt
            GPIO_enableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //enable analyze button interrupt
        }

        else
        {
            //Read ADC Voltages for photoresistor
            //Act upon results of read adc voltage
            //Set timer for 170 additional seconds
            //If no error continue remaing 2:50s of sanitization
            TA0CCR0 = 106249; // at 625 Hz, set 170 second timer and then check UVC Voltages (170s * 625 Hz =106250)
            TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;
            UVCCheck = 0; //UVC Check is finished
            GPIO_setOutputLowOnPin(GreenLEDNOTPort, GreenLEDNOTPin);
        }
    }


    // Hardware clears the flag, CCIFG in TA0CCTL0
}







