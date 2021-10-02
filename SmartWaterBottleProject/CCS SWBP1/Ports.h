/*
 * Ports.h
 *
 *  Created on: Aug 23, 2021
 *      Author: User
 */
#include <msp430.h>
#include <stdio.h>
#include <driverlib.h>

#ifndef PORTS_H_
#define PORTS_H_

//-------------------------------------------------------------------------//
//***********Define Statements*********************************************//

//--------------LEDs----------------------------------------//
#define redLED    GPIO_PIN0    //Red LED at P1.0 for launchpad
#define greenLED  GPIO_PIN7    //Green LED at  P9.7
#define yellowLED GPIO_PIN5    //Yellow LED at P9.5
#define blueLED   GPIO_PIN6
#define UVC       GPIO_PIN4    //Enable UVC at P1.4

/*
#define redLED    GPIO_PIN0    //Red LED at P3.0 for 5989
#define greenLED  GPIO_PIN2    //Green LED at  P3.2 for 5989
#define yellowLED GPIO_PIN1    //Yellow LED at P3.1 for 5989
#define blueLED   GPIO_PIN3    //Blue LED at P5.3 for 5989
#define UVC       GPIO_PIN4    //Enable UVC at P1.4
 */

//--------------Buttons-------------------------------------//
#define sanitizebutton     GPIO_PIN1     //P1.1 for 5989 AND launchpad
#define waterqualitybutton GPIO_PIN2     //P1.2 for 5989 AND launchpad



//-------------Enable Pins-----------------------------------//
#define battreadenable     GPIO_PIN5     //Enable Battery read at P1.5 for 5989
#define uvcenable     GPIO_PIN4     //Enable UVC LED at P1.4 for 5989
#define motorenable     GPIO_PIN7     //Enable motor at P1.7 for 5989
#define photoresistorenable     GPIO_PIN7     //Enable photoresistor at P9.7 for 5989
#define greenlaserhighenable     GPIO_PIN5     //Enable Battery read at P2.5 for 5989


//-------------ADC Inputs-----------------------------------//



#endif /* PORTS_H_ */
