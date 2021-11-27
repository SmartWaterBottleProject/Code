/*
 * BatteryRead.c
 *
 *  Created on: Aug 23, 2021
 *      Author: User
 */
#include <Batteryreader.h>
#include <Def.h>
#include <stdint.h>

//Return Batt % to main.c
//Run ADC to check voltage and return charge as integer percentage
uint8_t Batteryread(void){

//    uint16_t MaxVoltageADC = 4095;  //4.2V Max voltage converted to 12 bit ADC reading
//    uint16_t EOLVoltageADC = 3314;  //3.4V EOL voltage converted to 12 bit ADC reading
//    uint16_t MaxMinusEOL = MaxVoltageADC-EOLVoltageADC;  //Take difference for simplifying percentage calculations
    uint8_t BatteryPercentage = 0; //Variable for storing final battery percentages (calculated) as 8 bit integer
    BatteryPercentage = 0;

    GPIO_setOutputHighOnPin(BatteryReadEnablePort, BatteryReadEnablePin);  //Enable Battery Read
    Initialize_ADC_BatteryRead();  //Initialize ADC for battery read;



            ADC12CTL0 |= ADC12SC; //set, start conversion for adc
            //wait for flag to clear
            while( (ADC12CTL1 & ADC12BUSY) != 0 ){} //wait here, use !=0, since there could be other bits in bit field
            uint16_t BatteryRead = ADC12MEM0; //Result from current battery read
            ADC12CTL0 &= ~ADC12ON;  //Turn ADC off? Maybe leaving it on causes, current draw.
            REFCTL0 &= ~REFON;  //Disable internal reference for ADC

            //Convert ADC reading to a %
            uint32_t TempBattPerc = 0;
            TempBattPerc = (BatteryRead-3314);
            TempBattPerc = TempBattPerc*100;
            TempBattPerc = TempBattPerc/781;

            BatteryPercentage = (uint8_t)TempBattPerc;  //Store in 8 bit integer for percentage

//            if( BatteryRead <= EOLVoltageADC)
//            {
//                BatteryPercentage = 0;
//            }
//
//            else
//            {
////                BatteryPercentage = (uint16_t)100*(BatteryRead-3314)/(4095-3314);
//
//            }

    GPIO_setOutputLowOnPin(BatteryReadEnablePort, BatteryReadEnablePin);  //Disable Battery Read
    return BatteryPercentage;
}

