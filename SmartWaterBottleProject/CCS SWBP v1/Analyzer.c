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

//Something might be wrong with continue and break in configuration loop

//Treat voltage as adc integer or convert back to voltage

//Could eventually take multiple channel measurements of photodiode voltage

#define ZERO 0   //Value below expected peak ADC reading in LPM was previously 65535
#define ONE 1        //Allows for changing polarity of the motor
#define POSTZERO 80  //How many positions from zero the stepper should move after zero-ing run
#define BASELINE 0  //What can be considered a zero-voltage, experimentally
#define WIDTH 0  //Width of photodiode's element divided by step length of stepper motor

int Step(int);  //Steps the stepper motor over the input number of times

uint16_t GetVoltage(void);  //Function to take ADC measurement

uint16_t * Analyze(void)
{

    uint16_t MAX = ZERO;      //Measured Max. value from ADC read
    uint16_t Voltage;         //Stores voltage from ADC
    uint16_t BASELINE=0;
    uint16_t i=0;  //For loop counter that runs WIDTH times during analysis
    uint16_t j=0;  //For loop counter
    bool PassedPeak = 0;      //Have we passed the peak, 0-no, 1-yes


//********************Zeroing Run***********************************************************//

    //Turn on laser at low power setting
    GPIO_setOutputHighOnPin(LDLowPowerEnablePort, LDLowPowerEnablePin);  //Turn on Laser Diode
    Voltage = GetVoltage();  //Take ADC Measurement

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

           GPIO_setOutputHighOnPin(LDHighPowerEnablePort, LDHighPowerEnablePin);  //Add additional power for laser diode

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
              SAMPLES = (uint16_t *)calloc(POSITION, sizeof(uint16_t)); //Dynamically allocate memory for samples array, size of position
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

                  for(i=WIDTH; i<= POSITION; i++)  //For loop that iterates from WIDTH to POSITION using i
                  {
                      Voltage = GetVoltage();
                      SAMPLES[i] = Voltage;

                      for(j=i-1; j > i-WIDTH+1; j--)
                      {
                          SAMPLES[i] = SAMPLES[i] - SAMPLES[j];
                      }
                  }
              }

           return;
}





//    TB0CCR0=20000-1; //@1 Mhz, 20 millisecond
//    TB0CTL= TASSEL_2 | ID_0 | MC_1 | TBCLR;
//    TB0CCTL3 |= OUTMOD_7;     //Channel Reset/Set   ---these two lines might be wrong
//    TB0CCR3 = Start;  //Start PWM?



//********************Analysis Run***********************************************************//
        //Finding Peak of low power mode

    //    //Configure both Low power and High power LD at same time
    //    P1DIR |= LDHighPowerEnablePin;
    //    P1SEL1 |= LDHighPowerEnable;
    //    P1SEL0 |= LDHighPowerEnable;


int Step(int MoveBy)
{
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








////Analyzer Program Pseudocode
//
////Define a constant named ZERO to represent a value just below the expected zero'd value (determined experimentally)
////Define a constant named POSTZERO.  This represents how many positions from zero the stepper should move after the zeroing run (determined experimentally)
////Define a constant named BASELINE.  This represents what can be considered a zero-voltage (determined experimentally).
////Define a constant named WIDTH.  This represents the number of steps needed to move the photodiode entirely past a sample (determined experimentally).
//	//To clarify, this is the width of the photodiode's element divided by the step length of the stepper motor.
////Define ONE as 1.  This represents a single step.
//	//This may look dumb, but it lets us switch the directly we want the program to iterate later on with fewer alterations.
//
//
////****************************************Performed in Main*********************************/
////Button pressed
//	//Pre-run check
//		//Check the reed switch to determine if the cap is closed
//		//Check the battery level to determine if there is probably enough energy for a run
////****************************************Performed in Main*********************************/
//
//	//Analyzer Operation
//		//If stepper
//			//Zeroing-Run
//				//Turn on laser to the low power setting
//				//Initialize MAX = ZERO, PEAK = 0;
//				//Get VOLT
//				//If the photodiode's voltage (VOLT) is more than half of MAX, step down by POSTZERO.
//				//Infinite While loop
//					//Step the stepper motor up
//					//Get VOLT
//					//Compare the photodiode's voltage to the MAX variable.
//						//If (MAX < VOLT) then MAX = VOLT and PEAK = 1;
//						//If (MAX > VOLT && PEAK == 1) exit the loop.  This means that we have passed the maximum point (the zero).
//				//int POSITION = Step(POSTZERO).  This moves the stepper POSTZERO positions and records how far it is from the zero.
//			//Analysis run
//				//Infinite While Loop
//					//Infinite While Loop
//						//Get VOLT
//						//If VOLT is greater than BASELINE
//							//POSITION += Step(-ONE);
//						//If the VOLT is equal to or below BASELINE, break.
//					//Allocate memory for an integer array that is POSITION units long named Samples.
//					//Step down by 2*WIDTH units.
//					//Initialize unsigned integer variables BaselineMax = 0, BaselineMin = -1 (overflows to the max value), Baseline.
//					//Baseline = VOLT/WIDTH (gets the average entry weight)
//					//For loop that runs WIDTH times
//						//Position += Step(ONE);
//						//Get VOLT
//						//If VOLT > BaselineMax, BaselineMax = VOLT
//						//If VOLT < BaselineMin, BaselineMin = VOLT
//					//If BaselineMax - BaselineMix > Baseline 	--This means that there is light incident at some point.
//						//Free Samples (if using allocated memory)
//						//Step down by 2*WIDTH times
//						//Continue;		--Jumps back to the beginning and starts the anaylsis over from the new position.
//					//Break;		--Exits the configuration while-loop and starts actually analyzing.
//				//For loop that iterates from 0 to WIDTH using i.
//					//Get VOLT
//					//Samples[i] = VOLT - (WIDTH-1-i)*Baseline;
//					//for(j=i-1;j>=0;j--)
//						//Samples[i] = Samples[i] - Samples[j];
//				//For Loop that iterates from WIDTH to POSITION using i.
//					//Get VOLT
//					//Samples[i] = VOLT;
//					//for(j=i-1;j>i-WIDTH+1;j--)
//						//Samples[i] = Samples[i] - Samples[j];
//				//--Now we have a vector of samples


//Once finished call exporter to send data via bluetooth
//Servo may be utilized later
