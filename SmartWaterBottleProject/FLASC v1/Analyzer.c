///*
// * Analyze.c
// *
// *  Created on: Aug 23, 2021
// *      Author: User
// */
#include <Analyzer.h>
#include <Def.h>
#include <stdio.h>
#include <ADC.h>
#include <stdlib.h>

//Min pulse width for stepper is 1us
//Treat voltage as adc integer or convert back to voltage
//Could eventually take multiple channel measurements of photodiode voltage

#define ZERO 0   //Value below expected peak ADC reading in LPM was previously 65535
#define ONE 1        //Allows for changing polarity of the motor
#define POSTZERO 80  //How many positions from zero the stepper should move after zero-ing run
#define BASELINE 0  //What can be considered a zero-voltage, experimentally
#define WIDTH 0  //Width of photodiode's element divided by step length of stepper motor
int LDPower; 	 //Global laser power level

int Step(int);  //Steps the stepper motor over the input number of times
void Laser(int); //Sets the power level
void Wait(int); //Waits a certain number of milliseconds.
uint16_t GetVoltage(void);  //Function to take ADC measurement
void Dir(int); //Sets direction of stepper motor

bool Analyze()
{

    uint16_t MAX = ZERO;      //Measured Max. value from ADC read
    uint16_t Voltage=0;         //Stores voltage from ADC
    uint16_t i=0;  //For loop counter that runs WIDTH times during analysis
    uint16_t j=0;  //For loop counter
    bool PassedPeak = 0;      //Have we passed the peak, 0-no, 1-yes
    bool AnalyzerResult =0; //Boolean result returned by analyzer, sample is bad by default


    GPIO_setOutputLowOnPin(BlueLEDNOTPort, BlueLEDNOTPin);  //Turn blue LED on



//********************Zeroing Run***********************************************************//





    //Turn on laser at low power setting
    Laser(1);  //Turn on Laser Diode

    Voltage = GetVoltage();  //Take ADC Measurement
    int x=0;
    for(x; x<10000; x++){}

    if(Voltage >= 0.5*MAX )
    {
     Step(POSTZERO);  //Stepdown by postzero steps
    }

    while(1)
    {
        Step(ONE);  //Step the stepper motor up one step
        Voltage = GetVoltage(); //Get Voltage measurement from Laser diode
        //Compare photodiode VOLT to MAX
               if(MAX <= Voltage)
               {
                   MAX = Voltage;
                   PassedPeak =1;
               }
               if(MAX > Voltage && PassedPeak)  //We have passed the maximum point (the zero)
               {
                   break;
               }
           }

           int Position = Step(POSTZERO);  //Move the stepper POSTZERO positions and record distance from Zero



//********************Analysis Run***********************************************************//

           Laser(2);  //Add additional power for laser diode

           for(;;)
           {
               for(;;)
               {
                 Voltage = GetVoltage();  //Take ADC measurement
                 if (Voltage > BASELINE)
                 {
                     Position += Step(-ONE);
                 }

                 if(Voltage<= BASELINE)
                 {
                     break;
                 }
               }

               //Dynamic memory allocation for SAMPLES, here
              uint16_t * SAMPLES;
              SAMPLES = (uint16_t *)calloc(Position, sizeof(uint16_t)); //Dynamically allocate memory for samples array, size of position
              Step(WIDTH*2);  //Step down by Width*2 Units

              //Initialize Baseline variables
              uint16_t BaselineMax = 0;
              uint16_t BaselineMin = 4096-1;  //Just make max value for 12-bit ADC result
              uint16_t Baseline;  //Variable used to store average entry weight
              Baseline = Voltage/WIDTH;  //Gets average entry weight

              for(i=0; i<WIDTH; i++)  //For loop that runs Width times
              {
                  Position += Step(ONE);
                  Voltage = GetVoltage(); //Get Voltage measurement from Laser diode

                  if(Voltage > BaselineMax)
                  {
                      BaselineMax = Voltage;
                  }
                  if(Voltage < BaselineMin)
                  {
                      BaselineMin = Voltage;
                  }

              }

              if((BaselineMax - BaselineMin) > Baseline)  //Light is incident at some point
              {
                  free(SAMPLES);  //Free dynamically allocated memory used for samples
                  Step(2*WIDTH);  //Step down by 2*WIDTH times
                  continue;  //Jump back to beginning and start analysis over from new position

              }

              break;  //Exit the configuration while loop

              //Now start analyzing
              for(i=0; i< WIDTH; i++)  //For loop that iterates from 0 to WIDTH using i
              {
                  Voltage = GetVoltage();
                  SAMPLES[i] = Voltage - (WIDTH-1-i)*Baseline;

                  for(j=i-1; j>=0; j--)
                  {
                      SAMPLES[i] = SAMPLES [i] - SAMPLES[j];
                  }
              }

                  for(i=WIDTH; i<= Position; i++)  //For loop that iterates from WIDTH to POSITION using i
                  {
                      Voltage = GetVoltage();
                      SAMPLES[i] = Voltage;

                      for(j=i-1; j > i-WIDTH+1; j--)
                      {
                          SAMPLES[i] = SAMPLES[i] - SAMPLES[j];
                      }
                  }
              }

           GPIO_setOutputLowOnPin(StepperSleepNotPort, StepperSleepNotPin);  //Put motor back to sleep
			Laser(0);
           return AnalyzerResult;
}

int Step(int MoveBy)
{
    int i=0;  //Counter for for loop
    //Check direction
    if(MoveBy >= 0 )  //If MoveBy is positive
    {
        GPIO_setOutputHighOnPin(StepperDirectionPort, StepperDirectionPin);  //Set as positive direction
    }

    if(MoveBy < 0)  //If Moveby is negative
    {
        GPIO_setOutputLowOnPin(StepperDirectionPort, StepperDirectionPin);  //Set as negative direction
        MoveBy = -MoveBy;  //Make positive for count
    }

    //Initialize timer to 1 kHz
    // Timer_A: SMCLK, div by 1, up mode, clear TAR (leaves TAIE=0)
    TA0CCR0 = 500-1; // at 1 MHz, set .001 second timer (1kHz), for each rising edge at 1kHz frequency
    TA0CTL = TASSEL_2 | ID_0 | MC_1 | TACLR;
    TA0CTL &= ~TAIFG;  //Clear flag at start

    for(i=0; i<2*MoveBy; i++)
    {
        while((TA0CTL & TAIFG) == 0){}
        TA0CTL &=~TAIFG;
        GPIO_toggleOutputOnPin(MotorStepPort, MotorStepPin);  //Toggle pin
    }

    TA0CTL = MC_0;  //Turn timer off

	//Move the stepper motor up or down the MoveBy number of times.
	return MoveBy;
}

//Get the voltage from the ADC, returns 16 bit integer.
uint16_t GetVoltage()
{
    Initialize_ADC_Photodiode();  //Initialize the ADC
    ADC12CTL0 |= ADC12SC; //set, start conversion for adc
    //wait for flag to clear
    while( (ADC12CTL1 & ADC12BUSY) != 0 ){} //wait here, use !=0, since there could be other bits in bit field
    uint16_t PhotodiodeVoltage = ADC12MEM0;
    ADC12CTL0 &= ~ADC12ON;  //Turn ADC off? Maybe leaving it on causes, current draw.

	return PhotodiodeVoltage;
}

void Laser(int LDPowerLevelTo){ //This system assumes a valid operation.  The logic was confirmed to work in C terminal.
    if(LDPowerLevelTo<0||LDPowerLevelTo>2) return; //Error handling
    if(LDPowerLevelTo == 0){
        if(LDPower == 1){
            GPIO_setOutputLowOnPin(LDLowPowerEnablePort, LDLowPowerEnablePin);
        }
        if(LDPower == 2){
            GPIO_setOutputLowOnPin(LDHighPowerEnablePort, LDHighPowerEnablePin);
            GPIO_setOutputLowOnPin(LDLowPowerEnablePort, LDLowPowerEnablePin);
        }
    }
    if(LDPowerLevelTo == 1){
        if(LDPower == 0){
            GPIO_setOutputHighOnPin(LDLowPowerEnablePort, LDLowPowerEnablePin);
        }
        if(LDPower == 2){
            GPIO_setOutputLowOnPin(LDHighPowerEnablePort, LDHighPowerEnablePin);
        }
    }
    if(LDPowerLevelTo == 2){
        if(LDPower == 0){
            GPIO_setOutputHighOnPin(LDLowPowerEnablePort, LDLowPowerEnablePin);
            GPIO_setOutputHighOnPin(LDHighPowerEnablePort, LDHighPowerEnablePin);
        }
        if(LDPower == 1){
            GPIO_setOutputHighOnPin(LDHighPowerEnablePort, LDHighPowerEnablePin);
        }
    }
    LDPower = LDPowerLevelTo;
}
void Wait(int time){ //Waits a specified number of milliseconds
    int t;
    TA0CCR0 = 10000-1;
    TA0CTL = TASSEL_2 | ID_0 | MC_1 | TACLR;
    TA0CTL &= ~TAIFG;  //Clear flag at start
    for(t=0; t<time; t++)
    {
        while((TA0CTL & TAIFG) == 0){}
        TA0CTL &=~TAIFG;
    }
    TA0CTL = MC_0;  //Turn timer off
}
void Dir(int polarity){ //Sets the direction of the stepper motor.
    if(polarity>0) GPIO_setOutputLowOnPin(StepperDirectionPort, StepperDirectionPin);  //Set as Negative direction
    else GPIO_setOutputHighOnPin(StepperDirectionPort, StepperDirectionPin);  //Set as positive direction
}
//Once finished call exporter to send data via bluetooth


//Servo may be utilized later
