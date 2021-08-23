/*
 * Ports.h
 *
 *  Created on: Aug 23, 2021
 *      Author: User
 */

#ifndef PORTS_H_
#define PORTS_H_

//-------------------------------------------------------------------------//
//***********Define Statements*********************************************//

//--------------LEDs----------------------------------------//
#define redLED    BIT0    //Red LED at P9.0
#define greenLED  BIT1    //Green LED at  P9.7
#define yellowLED BIT5    //Yellow LED at P9.5
#define blueLED   BIT6
#define UVC       BIT4    //Enable UVC at P1.4

//--------------Buttons-------------------------------------//
#define sanitizeButton     BIT1     //P1.1
#define waterQualityButton BIT2     //P1.2


//-------------ADC Inputs-----------------------------------//
#define battReadEnable     BIT5     //Enable Battery read at P1.5




#endif /* PORTS_H_ */
