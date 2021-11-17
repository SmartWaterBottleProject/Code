/*
 * BatteryRead.c
 *
 *  Created on: Aug 23, 2021
 *      Author: User
 */
#include <Batteryreader.h>
#include <Def.h>
#include <ADC.h>

//ADC read on Battery voltage
    //Compare to 2-d array (hash table) of voltages to determine device battery %, rounded to the nearest 5% (voltage,%)
    //Update current Batt % value

//Return Batt % to main.c

int Batteryread(uint8_t BatteryLife)
{
    Initialize_ADC_BatteryRead();  //Configure ADC for battery read

    return BatteryLife;
}

