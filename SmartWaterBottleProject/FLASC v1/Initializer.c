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

       //initialize clock signal of Aux clock to VLO clock (5 MHz) divided by 1 = 625 Hz
       CS_initClockSignal (CS_SMCLK, CS_MODOSC_SELECT, CS_CLOCK_DIVIDER_1);

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
//       GPIO_setAsOutputPin(StepperDirectionPort, StepperDirectionPin);  //Configure stepper direction as output
//       GPIO_setOutputHighOnPin(StepperDirectionPort, StepperDirectionPin);  //Assert + or high by default
//       GPIO_setAsOutputPin(MotorEnablePort, MotorEnablePin);            //Configure Motor Enable as output
//       GPIO_setOutputLowOnPin(MotorEnablePort, MotorEnablePin);         //ENABLE motor by default
//       GPIO_setAsOutputPin(StepperSleepNotPort, StepperSleepNotPin);          //Configure stepper sleep as output
//       GPIO_setOutputLowOnPin(StepperSleepNotPort, StepperSleepNotPin);       //For enable configuration--set as low and disable stepper (stepper motor is asleep, approx. 1uA current)
       GPIO_setAsOutputPin(LDLowPowerEnablePort, LDLowPowerEnablePin);  //Configure LD Low Power mode enable pin as output
       GPIO_setOutputLowOnPin(LDLowPowerEnablePort, LDLowPowerEnablePin);   //Configure as low by default, since active high
       //Also enable high power branch, to access additional power for Laser Diode
//       GPIO_setAsOutputPin(LDHighPowerEnablePort, LDHighPowerEnablePin);    //Configure LD High Power mode enable pin as output
//       GPIO_setOutputLowOnPin(LDHighPowerEnablePort, LDHighPowerEnablePin); //Configure as low by default, since active high
//       GPIO_setAsOutputPin(MotorStepPort, MotorStepPin);  //Configure Motorstep as output
//       GPIO_setOutputLowOnPin(MotorStepPort, MotorStepPin);  //Set low by default



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

//--------------Bluetooth Module / UART------------------------------------------------------------------------------------------------------//

       GPIO_setAsOutputPin(BluetoothCYSPPPort, BluetoothCSYPPPin);  //Set as output
       GPIO_setOutputLowOnPin(BluetoothCYSPPPort, BluetoothCSYPPPin);  //Set as low

              // Divert pins to UART functionality
                  P2SEL1 &= ~(BIT0|BIT1);
                  P2SEL0 |= (BIT0|BIT1);

                  // Configure UART
                  EUSCI_A_UART_initParam param = {0};
                  param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;

                  //These values calculated according to family user's guide, pdf p.779
                  param.clockPrescalar = 2;
                  param.firstModReg = 11;
                  param.secondModReg = 0x92;
                  param.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION; //OS16 = 1

                  //These values confirmed in Cypress EZ-Serial firmware guide
                  param.parity = EUSCI_A_UART_NO_PARITY;
                  param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
                  param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
                  param.uartMode = EUSCI_A_UART_MODE;



                  /*if (STATUS_FAIL == EUSCI_A_UART_init(EUSCI_A0_BASE, &param)) {
                  return; // error handling suggested by driverlib guide
                  }*/
                  EUSCI_A_UART_init(EUSCI_A0_BASE, &param);



//-------------ADC Inputs (Pulled high by default)------------------------------------------------------------------------------------------------------//
       // Divert the pins to analog functionality
         P9SEL1 |= (BIT0&BIT1);
         P9SEL0 |= (BIT0&BIT1);

         P1SEL1 |= BIT3;
         P1SEL0 |= BIT3;





//***************************unused***********************************//

         GPIO_setAsInputPin(GPIO_PORT_P4, GPIO_PIN3);        //P4.3
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN3);

         GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN6);       //P1.6
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN6);

         GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN7);       //P1.7
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN7);

         GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN5);       //P2.5
         GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, GPIO_PIN5);

         GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN6);       //P2.6
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN6);

         GPIO_setAsInputPin(GPIO_PORT_P5, GPIO_PIN0);       //P5.0
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN0);

         GPIO_setAsInputPin(GPIO_PORT_P5, GPIO_PIN1);       //P5.1
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN1);

         GPIO_setAsInputPin(GPIO_PORT_P5, GPIO_PIN2);       //P5.2
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN2);

         GPIO_setAsInputPin(GPIO_PORT_PJ, GPIO_PIN0);        //PJ.0
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_PJ, GPIO_PIN0);

         GPIO_setAsInputPin(GPIO_PORT_PJ, GPIO_PIN1);        //PJ.1
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_PJ, GPIO_PIN1);

         GPIO_setAsInputPin(GPIO_PORT_PJ, GPIO_PIN2);        //PJ.2
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_PJ, GPIO_PIN2);

         GPIO_setAsInputPin(GPIO_PORT_PJ, GPIO_PIN3);        //PJ.3
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_PJ, GPIO_PIN3);

         GPIO_setAsInputPin(GPIO_PORT_PJ, GPIO_PIN4);        //PJ.4
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_PJ, GPIO_PIN4);

         GPIO_setAsInputPin(GPIO_PORT_PJ, GPIO_PIN5);        //PJ.5
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_PJ, GPIO_PIN5);

         GPIO_setAsInputPin(GPIO_PORT_PJ, GPIO_PIN6);        //PJ.6
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_PJ, GPIO_PIN6);

         GPIO_setAsInputPin(GPIO_PORT_PJ, GPIO_PIN7);        //PJ.7
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_PJ, GPIO_PIN7);

         GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN3);        //P3.3
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN3);

         GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN4);        //P3.4
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN4);

         GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN5);        //P3.5
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN5);

         GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN6);        //P3.6
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN6);

         GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN7);        //P3.7
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN7);

         GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN3);       //P2.3
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN3);

         GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN2);       //P2.2
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN2);

         GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN0);       //P1.0
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN0);

         GPIO_setAsInputPin(GPIO_PORT_P4, GPIO_PIN2);       //P4.2
         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN2);

int i=0;
for(i; i<10000; i++){}
       GPIO_setOutputHighOnPin(GreenLEDNOTPort, GreenLEDNOTPin);  //Turn green LED off after init.

       return;
}

