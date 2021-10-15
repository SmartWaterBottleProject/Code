/*
 * Reed.c
 *
 *  Created on: Oct 12, 2021
 *      Author: User
 */

#include <Def.h>
#include <Reed.h>

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

//    bool ReedOpen2 = 0;  //Cap is off at beginning of this function



//All interrupts should be in main
//Interrupt for one second timer
//    //Reconfigure reed switch is pulled high input and, check value at input
//        if high: Cap is back on, ProcessRunningNot=1, ReedOpen=1
//        if low: reconfigure as pulled low input, and wait another second
//#pragma vector = TIMER0_A0_VECTOR // Link the ISR to the vector
//__interrupt void T0A0_ISR_2()
//{
//    //Clear Timer A0 flag
//       //TA0CCTL0 &= ~CCIE; // Disable Channel 0 CCIE bit
//       TA0CCTL0 &= ~CCIFG; // Clear Channel 0 CCIFG bit
//       GPIO_setAsInputPinWithPullUpResistor(ReedSwitchPort, ReedSwitchPin);//Reconfigure reed switch pin as pulled high input
//
//       if(GPIO_getInputPinValue(ReedSwitchPort, ReedSwitchPin) == 1)     //Check to see if cap was placed back on
//       {
//          ReedOpen2 = 1; //Cap is no longer off, switch is open again
//          LPM4_EXIT;  //Exit low power mode
//       }
//       else
//       {
//           GPIO_setAsInputPinWithPullDownResistor(ReedSwitchPort, ReedSwitchPin);//Reconfigure reed switch pin as pulled low input (eliminate current draw)
//           GPIO_toggleOutputOnPin(GreenLEDNOTPort, GreenLEDNOTPin);  //Toggle Green LED every second
//       }
//       // Hardware clears the flag, CCIFG in TA0CCTL0
//}

