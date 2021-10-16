///*
// * Analyze.c
// *
// *  Created on: Aug 23, 2021
// *      Author: User
// */
#include <Analyzer.h>
#include <Def.h>
//
//// Configure PWM Move motor
////Enable and disable LD
////Move Motor...
//
////Once finished call exporter to send data to bluetooth module
//
////Min pulse width for stepper is 1us
//
//
//#define ZERO 65535   //Value below expected peak ADC reading in LPM
//#define ONE 1        //Allows for changing polarity of the motor
//#define POSTZERO 80  //How many positions from zero the stepper should move after zero-ing run
//
//int Step(int);  //Steps the stepper motor over the input number of times
//
uint16_t * Analyze(void)
{
//    uint16_t MAX = ZERO;      //Measured Max. value from ADC read
//    uint16_t Voltage;         //Stores voltage from ADC
//    bool PassedPeak = 0;      //Have we passed the peak, 0-no, 1-yes
//
////    //Configure P2.4 as TB0.3 for PWM
////    P1DIR |= LDLowPowerEnablePin;
////    P1SEL1 |= LDLowPowerEnable;
////    P1SEL0 |= LDLowPowerEnable;
//
////********************Zeroing Run***********************************************************//
//    //Finding Peak of low power mode
//
//    TB0CCR0=20000-1; //@1 Mhz, 20 millisecond
//    TB0CTL= TASSEL_2 | ID_0 | MC_1 | TBCLR;
//    TB0CCTL3 |= OUTMOD_7;     //Channel Reset/Set   ---these two lines might be wrong
//    TB0CCR3 = Start;  //Start PWM?
//    //Get VOLT--ADC frequency same as laser 20ms--baseline
//    if(Voltage > (.5*MAX))
//    {
//        //Step down by postzero
//
//    }
//
//    while()  //This while loop may need modification
//    {
//        //Step the stepper motor up
//        //Get VOLT
//        //Compare photodiode VOLT to MAX
//        if(MAX < Voltage)
//        {
//
//        }
//        if(PassedPeak)  //We have passed the maximum point (the zero)
//        {
//            break;
//        }
//    }
//
//    int Position = Step(POSTZERO);  //Move the stepper POSTZERO positins and record distance from Zero
//
////********************Analysis Run***********************************************************//
//        //Finding Peak of low power mode
//
//    //    //Configure both Low power and High power LD at same time
//    //    P1DIR |= LDHighPowerEnablePin;
//    //    P1SEL1 |= LDHighPowerEnable;
//    //    P1SEL0 |= LDHighPowerEnable;
//
//    //Start PWM right away for both
//
//    while()
//    {
//
//    }
//
//
    return;
}
//
////int Step(int MoveBy){
////	//Move the stepper motor up or down the MoveBy number of times.
////	return MoveBy;
////}
////
////int GetVolt(){
////	//Get the voltage from the ADC
////	return VOLT;
////}
//
//
//
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
//
//
//
//
//
//
////Servo may be utilized later
//
//
//
//
//
//
