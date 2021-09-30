/*
 * Ports.h
 *
 *  Created on: Aug 23, 2021
 *      Author: User
 */
#include <msp430.h>
#include <stdio.h>

#ifndef PORTS_H_
#define PORTS_H_

//-------------------------------------------------------------------------//
//***********Define Statements*********************************************//

//--------------LEDs----------------------------------------//
#define redLED    BIT0    //Red LED at P1.0
#define greenLED  BIT7    //Green LED at  P9.7
#define yellowLED BIT5    //Yellow LED at P9.5
#define blueLED   BIT6
#define UVC       BIT4    //Enable UVC at P1.4

//--------------Buttons-------------------------------------//
#define sanitizebutton     BIT1     //P1.1
#define waterqualitybutton BIT2     //P1.2


//-------------ADC Inputs-----------------------------------//
#define battreadenable     BIT5     //Enable Battery read at P1.5




#endif /* PORTS_H_ */
