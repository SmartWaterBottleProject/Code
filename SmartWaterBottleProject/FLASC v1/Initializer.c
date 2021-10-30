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
#include <stdio.h>

void initialize(void) {
       WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
       PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins

//--------------Clock and Timers------------------------------------------------------------------------------------------------------//

       //initialize clock signal of Aux clock to VLO clock (10 kHz) divided by 16 = 625 Hz
       CS_initClockSignal (CS_ACLK, CS_VLOCLK_SELECT, CS_CLOCK_DIVIDER_16);

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
       GPIO_setOutputLowOnPin(GreenLEDNOTPort, GreenLEDNOTPin);  //Green LED indicator during initial power up
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
       GPIO_setOutputHighOnPin(StepperDirectionPort, StepperDirectionPin);  //Assert + or high by default
       GPIO_setAsOutputPin(MotorEnablePort, MotorEnablePin);            //Configure Motor Enable as output
       GPIO_setOutputLowOnPin(MotorEnablePort, MotorEnablePin);         //Disable motor by default
       GPIO_setAsOutputPin(StepperSleepNotPort, StepperSleepNotPin);          //Configure stepper sleep as output
       GPIO_setOutputLowOnPin(StepperSleepNotPort, StepperSleepNotPin);       //By default, stepper motor is asleep, approx. 1uA current
       GPIO_setAsOutputPin(LDLowPowerEnablePort, LDLowPowerEnablePin);  //Configure LD Low Power mode enable pin as output
       GPIO_setOutputLowOnPin(LDLowPowerEnablePort, LDLowPowerEnablePin);   //Configure as low by default, since active high
       //Also enable high power branch, to access additional power for Laser Diode
       GPIO_setAsOutputPin(LDHighPowerEnablePort, LDHighPowerEnablePin);    //Configure LD High Power mode enable pin as output
       GPIO_setOutputLowOnPin(LDHighPowerEnablePort, LDHighPowerEnablePin); //Configure as low by default, since active high
       GPIO_setAsOutputPin(MotorStepPort, MotorStepPin);  //Configure Motorstep as output
       GPIO_setOutputLowOnPin(MotorStepPort, MotorStepPin);  //Set low by default



//--------------Sanitization------------------------------------------------------------------------------------------------------//

       GPIO_setAsOutputPin(UVCEnablePort, UVCEnablePin);                                    //Set UVC enable as output
       GPIO_setOutputLowOnPin(UVCEnablePort, UVCEnablePin);                                 //Disable UVCs by default
       GPIO_setAsInputPin(ReedSwitchPort,ReedSwitchPin);                                    //Set reed switch as input
       GPIO_setAsInputPinWithPullUpResistor(ReedSwitchPort,ReedSwitchPin);                  //Configure reed (normally closed) switch as pulled high by default (will have constant current draw whenever lid is not attached)
       GPIO_clearInterrupt(ReedSwitchPort, ReedSwitchPin);                                  //clear reed switch interrupt
       GPIO_enableInterrupt(ReedSwitchPort,ReedSwitchPin);                                  //Enable interrupt for reed switch
       GPIO_selectInterruptEdge(ReedSwitchPort,ReedSwitchPin,GPIO_HIGH_TO_LOW_TRANSITION);  //Set falling edge for reed switch
       GPIO_setAsOutputPin(PhotoresistorEnablePort, PhotoresistorEnablePin);                //Set photoresistor enable as output
       GPIO_setOutputLowOnPin(PhotoresistorEnablePort, PhotoresistorEnablePin);




//--------------Battery Circuit------------------------------------------------------------------------------------------------------//

       GPIO_setAsOutputPin(BatteryReadEnablePort, BatteryReadEnablePin);        //Set battery read enable as output
       GPIO_setOutputLowOnPin(BatteryReadEnablePort, BatteryReadEnablePin);     //Disable Battery read EN by default

//--------------Bluetooth Module------------------------------------------------------------------------------------------------------//




//-------------ADC Inputs (Pulled high by default)------------------------------------------------------------------------------------------------------//
//       GPIO_setAsInputPinWithPullUpResistor(PhotodiodeReadPort, PhotodiodeReadPin);
//       GPIO_setAsInputPinWithPullUpResistor(PhotoresistorReadPort, PhotoresistorReadPin);
//       GPIO_setAsInputPinWithPullUpResistor(BatteryReadPort, BatteryReadPin);
       //Just set all as pulled high inputs for now??



int i=0;
for(i; i<10000; i++){}
       GPIO_setOutputHighOnPin(GreenLEDNOTPort, GreenLEDNOTPin);  //Turn green LED off after init.

       return;
}

