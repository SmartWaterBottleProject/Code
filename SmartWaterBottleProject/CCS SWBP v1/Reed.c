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
//                REED=1
//                ProcessRunningNot = 1
//        if (low) Reed switch is stil closed (shorted to 0V), disable pull up, and re-enable pull down


void reed(void)
{
//Test red LED to verify we made it to Reed
    GPIO_setOutputLowOnPin(GreenLEDNOTPort, GreenLEDNOTPin);  //Turn on Red LED

    return;
}
