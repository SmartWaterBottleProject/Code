/*
 * Sanitize.c
 *
 *  Created on: Aug 23, 2021
 *      Author: User
 */
#include <Def.h>
#include <Sanitizer.h>

bool sanitize; // need something for the timer interrupt vector to be able to modify and end while loop

//Start timer

//Turn ON LEDs

//Helper function to check Photoresistor voltage

//Turn OFF LEDs

//Re-enter LPM



void Sanitize(bool* safe, bool StartOrStop){ //takes the address of REED to have access to the status of the Reed switch
    //one sanitization cycle should be 3 minutes (625 Hz x 180 seconds = 112,500 for counter, which starts at zero)

    /* hey, the driverlib functions want me to use a struct just to initialize up mode, this is done in one line
     * of code from embedded systems

    Timer_A_clear(0340); //Timer A0 base address is 0340 in hex, taken from p.133 from MSP430 datasheet

    Timer_A_initUpMode(0340, Timer_A_initUpModeParam param);

    Timer_A_enableInterrupt(0340);
    */
    sanitize=1; //global variable for Santizer.c, initialized on here and toggled off by TimerA0

  //Start sanitizer
    if((*safe) && (!StartOrStop)){ // checks that reed switch is closed, cap is secure, dereferencing REED address

        TA0CCTL0 |= CCIE; // Enable Channel 0 CCIE bit
        TA0CCTL0 &= ~CCIFG; // Clear Channel 0 CCIFG bit


        // Timer_A: ACLK, div by 1, up mode, clear TAR (leaves TAIE=0)
        TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR;

        // Enable the global interrupt bit,
        //_enable_interrupts();

        GPIO_setOutputLowOnPin(YellowLEDNOTPort, YellowLEDNOTPin);  //Turn on Yellow sanitization LEDs
        GPIO_setOutputHighOnPin(UVCEnablePort, UVCEnablePin); // Enable UVCs

        return;  //Go back to main code, stay in LPM until sanitizer is done, cancelled, or cap is removed
    }




    //Need to add interrupt for Reed switch transitioning low (cap being removed from bottle) P2.7 LLWU Interrupt Vector
//    #pragma vector = PORT2_VECTOR  //Link the ISR to the Vector
//    __interrupt void P2_ISR()
//    {
//        if(P2IFG & ReedSwitchPin)  //Detect Reed Switch closing
//        {
//            //Stop Everything
//            GPIO_setOutputLowOnPin(UVCEnablePort, UVCEnablePin);        //Timer disables UVC LEDs
//            GPIO_setOutputHighOnPin(YellowLEDNOTPort, YellowLEDNOTPin); //Timer turns off yellow indicator LED
//        }
//    }


//        _low_power_mode_4();  //Re-enter LPM until sanitizer is done or cap is removed
//        while(sanitize && *safe){} //code waits here until timer shuts LEDs off, checks values of StartSanitize and REED
//    }
//    P1OUT &= ~GPIO_PIN0; // turn off  red LED

   // return;
}


//Moved vector for timer to main function



