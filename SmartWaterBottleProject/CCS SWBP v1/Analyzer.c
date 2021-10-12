/*
 * Analyze.c
 *
 *  Created on: Aug 23, 2021
 *      Author: User
 */
#include <Analyzer.h>
#include <Def.h>

// Configure PWM Move motor
//Enable and disable LD
//Move Motor...

//Once finished call exporter to send data to bluetooth module

int Step(int);

void Analyze(void){

    P9OUT ^= GPIO_PIN7; // Toggle the green LED

    return;
}

//int Step(int MoveBy){
//	//Move the stepper motor up or down the MoveBy number of times.
//	return MoveBy;
//}
//
//int GetVolt(){
//	//Get the voltage from the ADC
//	return VOLT;
//}



//Analyzer Program Pseudocode

//Define a constant named ZERO to represent a value just below the expected zero'd value (determined experimentally)
//Define a constant named POSTZERO.  This represents how many positions from zero the stepper should move after the zeroing run (determined experimentally)
//Define a constant named BASELINE.  This represents what can be considered a zero-voltage (determined experimentally). 
//Define a constant named WIDTH.  This represents the number of steps needed to move the photodiode entirely past a sample (determined experimentally).
	//To clarify, this is the width of the photodiode's element divided by the step length of the stepper motor.
//Define ONE as 1.  This represents a single step.
	//This may look dumb, but it lets us switch the directly we want the program to iterate later on with fewer alterations.

//Button pressed
	//Pre-run check
		//Check the reed switch to determine if the cap is closed
		//Check the battery level to determine if there is probably enough energy for a run
		
	//Analyzer Operation
		//If stepper
			//Zeroing-Run
				//Turn on laser to the low power setting
				//Initialize MAX = ZERO, PEAK = 0;
				//Get VOLT
				//If the photodiode's voltage (VOLT) is more than half of MAX, step down by POSTZERO.  
				//Infinite While loop
					//Step the stepper motor up
					//Get VOLT
					//Compare the photodiode's voltage to the MAX variable.
						//If (MAX < VOLT) then MAX = VOLT and PEAK = 1;
						//If (MAX > VOLT && PEAK == 1) exit the loop.  This means that we have passed the maximum point (the zero).
				//int POSITION = Step(POSTZERO).  This moves the stepper POSTZERO positions and records how far it is from the zero.
			//Analysis run
				//Infinite While Loop
					//Infinite While Loop
						//Get VOLT
						//If VOLT is greater than BASELINE
							//POSITION += Step(-ONE);
						//If the VOLT is equal to or below BASELINE, break.
					//Allocate memory for an integer array that is POSITION units long named Samples.
					//Step down by 2*WIDTH units.
					//Initialize unsigned integer variables BaselineMax = 0, BaselineMin = -1 (overflows to the max value), Baseline.
					//Baseline = VOLT/WIDTH (gets the average entry weight)
					//For loop that runs WIDTH times
						//Position += Step(ONE);
						//Get VOLT
						//If VOLT > BaselineMax, BaselineMax = VOLT
						//If VOLT < BaselineMin, BaselineMin = VOLT
					//If BaselineMax - BaselineMix > Baseline 	--This means that there is light incident at some point.
						//Free Samples (if using allocated memory)
						//Step down by 2*WIDTH times
						//Continue;		--Jumps back to the beginning and starts the anaylsis over from the new position.
					//Break;		--Exits the configuration while-loop and starts actually analyzing.
				//For loop that iterates from 0 to WIDTH using i.
					//Get VOLT
					//Samples[i] = VOLT - (WIDTH-1-i)*Baseline;
					//for(j=i-1;j>=0;j--)
						//Samples[i] = Samples[i] - Samples[j];
				//For Loop that iterates from WIDTH to POSITION using i.
					//Get VOLT
					//Samples[i] = VOLT;
					//for(j=i-1;j>i-WIDTH+1;j--)
						//Samples[i] = Samples[i] - Samples[j];
				//--Now we have a vector of samples
					
					
					
					
					
					
