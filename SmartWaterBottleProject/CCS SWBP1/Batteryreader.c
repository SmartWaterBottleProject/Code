/*
 * BatteryRead.c
 *
 *  Created on: Aug 23, 2021
 *      Author: User
 */
#include <Batteryreader.h>
#include <Ports.h>

//ADC read on Battery voltage
    //Compare to 2-d array (hash table) of voltages to determine device battery %, rounded to the nearest 5% (voltage,%)
    //Update current Batt % value

//Return Batt % to main.c

int Batteryread(void){

    //Run ADC to check voltage and return charge as integer percentage
    return 85;
}

