/*
 * Def.h
 *
 *  Created on: Aug 23, 2021
 *      Author: User
 */

//Ryan Koons and Dean Pickett

//Use for simplicity of reading the rest of the code base.
//Relating pins and ports from the Driverlib API to project function.

#include <msp430.h>
#include <stdbool.h>
#include <stdio.h>
#include <driverlib.h>
#include <intrinsics.h>


#ifndef PORTS_H_
#define PORTS_H_

//-------------------------------------------------------------------------//
//***********Define Statements*********************************************//
//-------------------------------------------------------------------------//


//--------------Indicator LEDs----------------------------------------//

//Note LEDs labelled "NOT" since they are active low. I.e. A low signal turns them on
#define RedLEDNOTPort      GPIO_PORT_P3  //Red LED at P3.0
#define RedLEDNOTPin       GPIO_PIN0

#define GreenLEDNOTPort    GPIO_PORT_P3  //Green LED at P3.2
#define GreenLEDNOTPin     GPIO_PIN2

#define BlueLEDNOTPort     GPIO_PORT_P5  //Blue LED at P5.3
#define BlueLEDNOTPin      GPIO_PIN3

#define YellowLEDNOTPort   GPIO_PORT_P3  //Yellow LED at P3.1
#define YellowLEDNOTPin    GPIO_PIN1



//--------------Buttons-------------------------------------//

#define SanitizeButtonPort      GPIO_PORT_P1  //Sanitize LLWU for Button on P1.1 (same as launchpad)
#define SanitizeButtonPin       GPIO_PIN1

#define AnalyzeButtonPort      GPIO_PORT_P1  //Analyze LLWU for Button on P1.2 (same as launchpad)
#define AnalyzeButtonPin       GPIO_PIN2


//-------------Enable Pins-----------------------------------//

#define UVCEnablePort              GPIO_PORT_P1  //Enable UVC at P1.4
#define UVCEnablePin               GPIO_PIN4

#define BatteryReadEnablePort      GPIO_PORT_P1  //Enable Battery Read at P1.5
#define BatteryReadEnablePin       GPIO_PIN5

#define MotorEnablePort            GPIO_PORT_P1  //Enable Motor at P1.7
#define MotorEnablePin             GPIO_PIN7

#define PhotoresistorEnablePort    GPIO_PORT_P9  //Enable Photoresistor at P9.7
#define PhotoresistorEnablePin     GPIO_PIN7

#define LDLowPowerEnablePort       GPIO_PORT_P2  //Enable Laser Diode (Low Power Mode) at P2.4
#define LDLowPowerEnablePin        GPIO_PIN4

#define LDHighPowerEnablePort       GPIO_PORT_P2  //Enable Laser Diode (High Power Mode) at P2.5
#define LDHighPowerEnablePin        GPIO_PIN5


//-------------Wireless Module Pins-----------------------------------//

#define BluetoothLPModePort         GPIO_PORT_P9  //Control Bluetooth Low Power mode at P9.2
#define BluetoothLPModePin          GPIO_PIN2

#define BluetoothDataReadyPort      GPIO_PORT_P9  //Bluetooth data ready at P9.3
#define BluetoothDataReadyPin       GPIO_PIN3

#define BluetoothCTSPort            GPIO_PORT_P9  //Bluetooth CTS at P9.4
#define BluetoothCTSPin             GPIO_PIN4

#define BluetoothLPModeStatusPort   GPIO_PORT_P9  //Bluetooth Low power mode status at P9.5
#define BluetoothLPModeStatusPin    GPIO_PIN5

#define BluetoothRTSPort            GPIO_PORT_P9  //Bluetooth RTS at P9.6
#define BluetoothRTSPin             GPIO_PIN6

#define BluetoothCYSPPPort          GPIO_PORT_P4  //Control Bluetooth Low Power mode at P4.0
#define BluetoothCSYPPPin           GPIO_PIN0

#define BluetoothConnectionPort     GPIO_PORT_P4  //Control Bluetooth Low Power mode at P4.1
#define BluetoothConnectionPin      GPIO_PIN1


//-------------Other Pins-----------------------------------//

#define StepperDirectionPort      GPIO_PORT_P4  //Change stepper direction using P4.3
#define StepperDirectionPin       GPIO_PIN3

#define StepperSleepPort          GPIO_PORT_P2  //Control stepper sleep using P2.6
#define StepperSleepPin           GPIO_PIN6

#define ReedSwitchPort            GPIO_PORT_P2  //Reed switch at P2.7
#define ReedSwitchPin             GPIO_PIN7




#endif /* Def_H_ */
