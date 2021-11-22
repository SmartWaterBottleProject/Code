//Smart Water Bottle Project Main Function
//Programmers: Dean Pickett, Ryan Koons, and Matthew Woodruff
//UCF SD
//8-23-21


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
bool StartSanitize = 0, StartAnalyze = 0, ReedOpen = 1, UVCCheck =0, ValidSample=0, AnalyzerResult=0; // Global variables to trigger sanitization, analyzing, and Reed safety shutoff
bool SecondUVCTimer = 0;  //When variable = 1, timer is on second iteration of sanitization. Done to account for 16-bit TA0CCR0 Overflow issues
bool ProcessRunningNot = 1;  //If process is currently running, do not run another one, 0-process is running, 1-process is NOT running
uint8_t BatteryPercentage =0;  //uint8_t to save program space, stores battery percentage (rounded to nearest integer)

void reed();  //Function for polling reed switch when cap is removed
void BlinkLight(int,int);  //Toggles various led, port and pin


int main (void)
{
    //Call Initializer.c, set pin-configuration
    initialize();

    //Call Batteryread
    BatteryPercentage = Batteryread();

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

        //For testing exporter
        Export(BatteryPercentage, 1, ValidSample);  //No valid sample, transmit batt % only
        Export(70,1,1);  //Valid: transmit % and good
        Export(53, 0, 1);  //Valid: transmit % and bad

        if(StartSanitize && ProcessRunningNot) //checks if sanitize button was pressed, and no other process is running
        {
            //Call Batteryread
            BatteryPercentage = Batteryread();

            if(BatteryPercentage>= 20)  //Ensure there is enough battery life, prior to starting sanitization
            {
            //Call Sanitizer.c to start sanitization
            ProcessRunningNot =0;  //Blocking term for process
            Sanitize(&ReedOpen);  //Call sanitization function to start sanitizer
            UVCCheck = 1;  //Signify UVC Check during first 10 seconds
//            StopSanitize =1;  //Next time this if statement executes, the sanitizer should stop--might not need this

            }

            else
            {
                //Should eventually Blink red LED for low battery
                BlinkLight(RedLEDNOTPort, RedLEDNOTPin);  //Toggle RED led
            }

           StartSanitize = 0;  //Clear sanitization mode, not enough battery life to perform
        }


        if(StartAnalyze && ProcessRunningNot) //checks if analyze button was pressed, and no other process is running
        {
            //Call Batteryread
           BatteryPercentage = Batteryread();

            if(BatteryPercentage >= 20) //Ensure there is enough battery life, prior to starting sanitization
            {
                ProcessRunningNot = 0;
                AnalyzerResult = Analyze(); //Call the analyzer function, and return the analyzer result

                if(AnalyzerResult)  //If sample passes analyzer (good)
                {
                    BlinkLight(GreenLEDNOTPort, GreenLEDNOTPin);
                    BlinkLight(GreenLEDNOTPort, GreenLEDNOTPin);
                }
                else  //Bad sample
                {
                    BlinkLight(RedLEDNOTPort, RedLEDNOTPin);
                    BlinkLight(RedLEDNOTPort, RedLEDNOTPin);
                }
                ProcessRunningNot = 1;  //Process is no longer running
                ValidSample=1; //There is now a valid sample
                GPIO_enableInterrupt(SanitizeButtonPort, SanitizeButtonPin);  //Re-enable sanitize button interrupt
                GPIO_enableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //Re-enable sanitize button interrupt
                //Call Analyzer.c
               // Analyze();

            }

            else
                        {
                            //Should eventually Blink red LED for low battery
                BlinkLight(RedLEDNOTPort, RedLEDNOTPin);
                        }


            StartAnalyze = 0; //reset variable for calling analyzer
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
        //Remove current Water quality result/sample
        //Disable interrupts for buttons
        ValidSample = 0;  //Sample is no longer valid
        GPIO_disableInterrupt(SanitizeButtonPort, SanitizeButtonPin);  //Disable interrupt for sanitization button
        GPIO_disableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);    //Disable interrupt for analysis button
        GPIO_setOutputHighOnPin(GreenLEDNOTPort, GreenLEDNOTPin);      //Turn all other LEDs off
        GPIO_setOutputHighOnPin(YellowLEDNOTPort, YellowLEDNOTPin);
        GPIO_setOutputHighOnPin(BlueLEDNOTPort, BlueLEDNOTPin);
        GPIO_setOutputLowOnPin(RedLEDNOTPort, RedLEDNOTPin);  //Turn on Red
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
            GPIO_setOutputHighOnPin(RedLEDNOTPort, RedLEDNOTPin);  //Turn red LED OFF
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


#pragma vector = PORT1_VECTOR  //Link the ISR to the Vector
__interrupt void P1_ISR()
{
    //Deactivate both buttons
    GPIO_disableInterrupt(SanitizeButtonPort, SanitizeButtonPin);
    GPIO_disableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);

//Noise filtering: wait 10 microseconds for coupling in buttons signals to settle
    TA0CCR0 = 10-1; // at 1 MHz, set .001 second timer (1kHz), for each rising edge at 1kHz frequency
    TA0CTL = TASSEL_2 | ID_0 | MC_1 | TACLR;
    TA0CTL &= ~TAIFG;  //Clear flag at start
    while((TA0CTL & TAIFG) == 0){}
    TA0CTL |= MC_0;  //Turn off timer
    TA0CTL &=~TAIFG;

   if(GPIO_getInputPinValue(SanitizeButtonPort, SanitizeButtonPin)) // Detect button 1--sanitization
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


  // }

   }

   else if(GPIO_getInputPinValue(AnalyzeButtonPort, AnalyzeButtonPin))
   {

       //Check for button hold, wait 1/2 second
           TA0CCR0 = 1200; // at 1 MHz, set 1 second timer
           TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;
           TA0CTL &= ~TAIFG;  //Clear flag at start
           while((TA0CTL & TAIFG) == 0){}
           TA0CTL |= MC_0;  //Turn off timer
           TA0CTL &=~TAIFG;

           //Button no longer being held
//           GPIO_setAsInputPin(AnalyzeButtonPort, AnalyzeButtonPin);  //Remove pull up to check pin status?
//           uint8_t ButtonResult = GPIO_getInputPinValue(AnalyzeButtonPort, AnalyzeButtonPin);

           if((P1IN&BIT1) == BIT1 )  //Button is no longer being pressed
           {
               StartAnalyze = 1; // set global analyze variable
           }

           else if((P1IN&BIT1) != BIT1 ) //Button is still being pressed
           {
               Export(BatteryPercentage, AnalyzerResult, ValidSample);  //Call the exporter
               BlinkLight(BlueLEDNOTPort, BlueLEDNOTPin);  //Toggle Blue LED
               GPIO_enableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //Re-enable analyzer button interrupt
           }

//           GPIO_setAsInputPinWithPullUpResistor(AnalyzeButtonPort, AnalyzeButtonPin);




      // ReedOpen = 0;// this is for testing Reed interrupt functionality on launchpad
       //Disable LPM
       //LPM4_EXIT;

       LPM4_EXIT;  //I think this is necessary?
       //Deactivate both buttons
      // GPIO_disableInterrupt(SanitizeButtonPort, SanitizeButtonPin);
      // GPIO_disableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);


       //P9OUT ^= (RedLED|GreenLED|YellowLED|BlueLED);  //Toggle LEDs, haven't messed with this yet, not
                                                       // sure why this is here

   }
//Clear both since signal coupling raises both interrupts
   GPIO_clearInterrupt(SanitizeButtonPort, SanitizeButtonPin);  //clear sanitize button interrupt
   GPIO_clearInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //clear analyze button interrupt


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
              TA0CCTL0 &= ~CCIFG; // Clear Channel 0 CCIFG bit
              TA0CCTL0 &= ~CCIE; // Enable Channel 0 CCIE bit
              TA0CTL = MC_0;  //Turn timer off
              GPIO_setOutputHighOnPin(RedLEDNOTPort, RedLEDNOTPin);  //Turn RED LED off
              LPM4_EXIT;  //Exit low power mode
           }
           else
           {
               GPIO_setAsInputPinWithPullDownResistor(ReedSwitchPort, ReedSwitchPin);//Reconfigure reed switch pin as pulled low input (eliminate current draw)
               GPIO_toggleOutputOnPin(RedLEDNOTPort, RedLEDNOTPin);  //Toggle Green LED every second
           }
    }

    else if (!UVCCheck)
    {

        if(SecondUVCTimer)
        {
            TA0CCTL0 &= ~CCIFG; // Clear Channel 0 CCIFG bit
            TA0CCR0 = 40625-1; // at 625 Hz, set 65 second timer (170s * 625 Hz =106250)
            TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;
            SecondUVCTimer = 0;  //Second UVC Timer is done

        }

        else
        {
            //    sanitize = 0; //end sanitize function
                        GPIO_setOutputLowOnPin(UVCEnablePort, UVCEnablePin);        //Timer disables UVC LEDs
                        GPIO_setOutputHighOnPin(YellowLEDNOTPort, YellowLEDNOTPin); //Timer turns off yellow indicator LED
                        GPIO_setOutputHighOnPin(GreenLEDNOTPort, GreenLEDNOTPin);  //Turn green LED off
                    //    GPIO_setOutputLowOnPin(GreenLEDNOTPort, GreenLEDNOTPin);  //This is only here to make sure this vector triggers in main and terminates UVCs
                        //Clear Timer A0 flag and disable interrupt
                        TA0CCTL0 &= ~CCIE; // Disable Channel 0 CCIE bit
                        TA0CCTL0 &= ~CCIFG; // Clear Channel 0 CCIFG bit
                        TA0CTL = MC_0;  //Turn timer off
                        ProcessRunningNot = 1; //Process is no longer running
                        GPIO_clearInterrupt(SanitizeButtonPort, SanitizeButtonPin);  //clear sanitize button interrupt
                        GPIO_clearInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);
                        GPIO_enableInterrupt(SanitizeButtonPort, SanitizeButtonPin); //enable sanitize button interrupt
                        GPIO_enableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //enable analyze button interrupt

        }

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
        REFCTL0 &= ~REFON;  //Disable internal reference for ADC
        GPIO_setOutputLowOnPin(PhotoresistorEnablePort, PhotoresistorEnablePin);  //Disable photoresistor

        //If UVC check fails
        if( PhotoresistorVoltage <= 2730)  //If Less than 1V, if +VCC=3.3V    (4096*1/3.3 -->1240)
        {
            GPIO_setOutputLowOnPin(UVCEnablePort, UVCEnablePin);  //Disable UVCs
            GPIO_setOutputHighOnPin(YellowLEDNOTPort, YellowLEDNOTPin);  //Turn off yellow indicator LED
            TA0CCTL0 &= ~CCIE; // Disable Channel 0 CCIE bit
            TA0CCTL0 &= ~CCIFG; // Clear Channel 0 CCIFG bit
            TA0CTL |= MC_0;  //Turn timer off
            ProcessRunningNot = 1; //Process is no longer running
            UVCCheck =0;  //UVC Check is done
            GPIO_clearInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //clear analyze button interrupt
            GPIO_clearInterrupt(SanitizeButtonPort, SanitizeButtonPin);  //clear sanitization button interrupt
            GPIO_enableInterrupt(SanitizeButtonPort, SanitizeButtonPin); //enable sanitize button interrupt
            GPIO_enableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //enable analyze button interrupt
            BlinkLight(RedLEDNOTPort, RedLEDNOTPin);  //Toggle Red LEDs

        }

        else
        {
            //Read ADC Voltages for photoresistor
            //Act upon results of read adc voltage
            //Set timer for 170 additional seconds
            //If no error continue remaing 2:50s of sanitization
            TA0CCTL0 |= CCIE; // Enable Channel 0 CCIE bit
            TA0CCTL0 &= ~CCIFG; // Clear Channel 0 CCIFG bit
            TA0CCR0 = 52500-1; // at 625 Hz, set 104 second timer (104s * 625 Hz =65536) Changed to 52500 to account for extra 20 second timing
            TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;
            SecondUVCTimer = 1;  //Start second iteration of UVC Timer
            UVCCheck = 0; //UVC Check is finished
            GPIO_setOutputLowOnPin(GreenLEDNOTPort, GreenLEDNOTPin);
        }
    }


    // Hardware clears the flag, CCIFG in TA0CCTL0
}

//Blink LED at port and pin, 5x (5 on, and 5 off), ending in off state
void BlinkLight(int Port, int Pin)  //Port and pin
{
    uint8_t i=0;
    uint16_t j=0;

    for(i; i<11; i++)
    {
        for(j=0; j<20000; j++){} //Wait here
        GPIO_toggleOutputOnPin(Port, Pin);
    }
    GPIO_setOutputHighOnPin(Port, Pin);  //Turn Red LED OFF to start
}






