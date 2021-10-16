/*
 * Initialize.c
 *
 *  Created on: Aug 23, 2021
 *      Author: DGP, RKK
 */

//Configure GPIO Pins, and set initial states
    //Setting up ADC reads
    //Initialize UART
//Set-up all of the timer modules

//Need to initialize all unused ports as pulled high inputs to avoid excess current draw***************************************************************


#include <Initializer.h>
#include <driverlib.h>
#include <Def.h>

void initialize(void) {
       WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
       PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

//--------------Clock and Timers------------------------------------------------------------------------------------------------------//

       //initialize clock signal of Aux clock to VLO clock (10 kHz) divided by 16 = 625 Hz
       CS_initClockSignal (CS_ACLK, CS_VLOCLK_SELECT, CS_CLOCK_DIVIDER_16);
       TA0CCR0 = 6249; //should be 112,499 to get 180 seconds at 625 Hz, testing at 10 seconds

       //check is Reed switch is pulled low and cap is secured

       //Sample using DriverLib--P4.3 o/p, this is all code for custom PCB
     /*  GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN3 );  */

//--------------Indicator LEDs------------------------------------------------------------------------------------------------------//

       GPIO_setAsOutputPin(RedLEDNOTPort, RedLEDNOTPin ); // set red LED as output
       GPIO_setAsOutputPin(GreenLEDNOTPort, GreenLEDNOTPin ); // set green LED as output
       GPIO_setAsOutputPin(BlueLEDNOTPort, BlueLEDNOTPin ); // set blue LED as output
       GPIO_setAsOutputPin(YellowLEDNOTPort, YellowLEDNOTPin ); // set yellow LED as output

       // turn Indicator LEDs off here, all active low
       GPIO_setOutputHighOnPin(RedLEDNOTPort, RedLEDNOTPin);
       GPIO_setOutputHighOnPin(GreenLEDNOTPort, GreenLEDNOTPin);
       GPIO_setOutputHighOnPin(BlueLEDNOTPort, BlueLEDNOTPin);
       GPIO_setOutputHighOnPin(YellowLEDNOTPort, YellowLEDNOTPin);



       // THESE 4 LINES ARE FOR THE LAUNCHPAD
       // TAKE NOTE, USING SET OUTPUT HIGH TURNS LEDs ON FOR LAUNCHPAD
//       GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0 ); // set red LED, P1.0, as output as output on launchpad
//       GPIO_setAsOutputPin(GPIO_PORT_P9, GPIO_PIN7 ); // set green LED, P9.7, as output on launchpad
//       GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0); // Turn red LED Off
//       GPIO_setOutputLowOnPin(GPIO_PORT_P9, GPIO_PIN7); // Turn green LED Off

//--------------Buttons------------------------------------------------------------------------------------------------------//

       //configuring buttons
       GPIO_setAsInputPinWithPullUpResistor(SanitizeButtonPort, SanitizeButtonPin); //set sanitize button as input
       GPIO_setAsInputPinWithPullUpResistor(AnalyzeButtonPort, AnalyzeButtonPin); //set sanitize button as input

       GPIO_enableInterrupt(SanitizeButtonPort, SanitizeButtonPin); //enable sanitize button interrupt
       GPIO_enableInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //enable analyze button interrupt

       GPIO_selectInterruptEdge(SanitizeButtonPort, SanitizeButtonPin, GPIO_HIGH_TO_LOW_TRANSITION); //set sanitize interrupt on falling edge
       GPIO_selectInterruptEdge(AnalyzeButtonPort, AnalyzeButtonPin, GPIO_HIGH_TO_LOW_TRANSITION);  //set analyze interrupt on falling edge

       GPIO_clearInterrupt(SanitizeButtonPort, SanitizeButtonPin);  //clear sanitize button interrupt
       GPIO_clearInterrupt(AnalyzeButtonPort, AnalyzeButtonPin);  //clear analyze button interrupt

//--------------Water Quality Analysis------------------------------------------------------------------------------------------------------//
       GPIO_setAsOutputPin(StepperDirectionPort, StepperDirectionPin);  //Configure stepper direction as output
       GPIO_setAsOutputPin(MotorEnablePort, MotorEnablePin);            //Configure Motor Enable as output
       GPIO_setOutputLowOnPin(MotorEnablePort, MotorEnablePin);         //Disable motor by default
       GPIO_setAsOutputPin(StepperSleepPort, StepperSleepPin);          //Configure stepper sleep as output
       GPIO_setOutputLowOnPin(StepperSleepPort, StepperSleepPin);       //By default, stepper motor is asleep, approx. 1uA current

//--------------Sanitization------------------------------------------------------------------------------------------------------//

       GPIO_setAsOutputPin(UVCEnablePort, UVCEnablePin);                                    //Set UVC enable as output
       GPIO_setOutputLowOnPin(UVCEnablePort, UVCEnablePin);                                 //Disable UVCs by default
       GPIO_setAsInputPin(ReedSwitchPort,ReedSwitchPin);                                    //Set reed switch as input
       GPIO_setAsInputPinWithPullUpResistor(ReedSwitchPort,ReedSwitchPin);                  //Configure reed (normally closed) switch as pulled high by default (will have constant current draw whenever lid is not attached)
       GPIO_clearInterrupt(ReedSwitchPort, ReedSwitchPin);                                  //clear reed switch interrupt
       GPIO_enableInterrupt(ReedSwitchPort,ReedSwitchPin);                                  //Enable interrupt for reed switch
       GPIO_selectInterruptEdge(ReedSwitchPort,ReedSwitchPin,GPIO_HIGH_TO_LOW_TRANSITION);  //Set falling edge for reed switch
       GPIO_setAsOutputPin(PhotoresistorEnablePort, PhotoresistorEnablePin);                //Set photoresistor enable as output




//--------------Battery Circuit------------------------------------------------------------------------------------------------------//

       GPIO_setAsOutputPin(BatteryReadEnablePort, BatteryReadEnablePin);        //Set battery read enable as output
       GPIO_setOutputLowOnPin(BatteryReadEnablePort, BatteryReadEnablePin);     //Disable Battery read EN by default

//--------------Bluetooth Module------------------------------------------------------------------------------------------------------//

       //Are bluetooth module pins input or output? idk







       return;
}

