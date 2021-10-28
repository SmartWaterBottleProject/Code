/*
 * ADC.c
 *
 *  Created on: Oct 21, 2021
 *      Author: User
 */
#include <msp430.h>

//Using photodiode analog input on Launchpad for time being

//Uses 0V and +VCC for ADC references

//Using 12 bits of resolution for ADC

//Currently photodiode and photoresistor use same adc pin for the time being



// P9.1/A9
void Initialize_ADC_Photoresistor() //function to init. adc
{
// Divert the pins to analog functionality
  P9SEL1 |= BIT1;
  P9SEL0 |= BIT1;

  ADC12CTL0 |= ADC12ON; // Turn on the ADC module
  ADC12CTL0 &= ~ADC12ENC;  // Turn off ENC (Enable Conversion) bit while modifying the configuration

//*************** ADC12CTL0 ***************/ //set number of cycles
    // Set ADC12SHT0 (select the number of cycles that you determined)
    ADC12CTL0 |= ADC12SHT0_10;  //Allow 512 cycles for capacitors to charge

    //*************** ADC12CTL1 ***************
    // Set ADC12SHS (select ADC12SC bit as the trigger)
    // Set ADC12SHP bit
    // Set ADC12DIV (select the divider you determined)
    // Set ADC12SSEL (select MODOSC)

    //ADC12SHS should be defaulted to use ADC, but just in case
    //Use larger divider to ensure that capacitors for ADC comparator charge up in time
    ADC12CTL1 |= ADC12SHS_0|ADC12SHP|ADC12DIV_7|ADC12SSEL_0;
//note: the last three value should all be defaults, but they are stil set just in case


    //*************** ADC12CTL2 ***************
    // Set ADC12RES (select 12-bit resolution)
    ADC12CTL2 |= ADC12RES_2; //select 12 bit resolution //should be default, and unsigned binary
    //Omit Binary unsigned bitmask to have 0.
    // Set ADC12DF (select unsigned binary format)
    //this value is default
    //ADC12DF=0;  //default

    //*************** ADC12CTL3 ***************
    // Leave all fields at default values

    //*************** ADC12MCTL0 ***************

    // Set ADC12VRSEL (select VR+=AVCC, VR-=AVSS)
    //should be default
   // ADC12VRSEL=0;
    ADC12MCTL0 |= ADC12INCH_9| ADC12VRSEL_0;
    // Set ADC12INCH (select channel A10)

    // Turn on ENC (Enable Conversion) bit at the end of the configuration
    ADC12CTL0 |= ADC12ENC;
    return;
    }

void Initialize_ADC_Photodiode() //function to init. adc
{
// Divert the pins to analog functionality
  P9SEL1 |= BIT1;
  P9SEL0 |= BIT1;

  ADC12CTL0 |= ADC12ON; // Turn on the ADC module
  ADC12CTL0 &= ~ADC12ENC;  // Turn off ENC (Enable Conversion) bit while modifying the configuration

//*************** ADC12CTL0 ***************/ //set number of cycles
    // Set ADC12SHT0 (select the number of cycles that you determined)
    ADC12CTL0 |= ADC12SHT0_10;  //Allow 512 cycles for capacitors to charge

    //*************** ADC12CTL1 ***************
    // Set ADC12SHS (select ADC12SC bit as the trigger)
    // Set ADC12SHP bit
    // Set ADC12DIV (select the divider you determined)
    // Set ADC12SSEL (select MODOSC)

    //ADC12SHS should be defaulted to use ADC, but just in case
    //Use larger divider to ensure that capacitors for ADC comparator charge up in time
    ADC12CTL1 |= ADC12SHS_0|ADC12SHP|ADC12DIV_7|ADC12SSEL_0;
//note: the last three value should all be defaults, but they are stil set just in case


    //*************** ADC12CTL2 ***************
    // Set ADC12RES (select 12-bit resolution)
    ADC12CTL2 |= ADC12RES_2; //select 12 bit resolution //should be default, and unsigned binary
    //Omit Binary unsigned bitmask to have 0.
    // Set ADC12DF (select unsigned binary format)
    //this value is default
    //ADC12DF=0;  //default

    //*************** ADC12CTL3 ***************
    // Leave all fields at default values

    //*************** ADC12MCTL0 ***************

    // Set ADC12VRSEL (select VR+=AVCC, VR-=AVSS)
    //should be default
   // ADC12VRSEL=0;
    ADC12MCTL0 |= ADC12INCH_9| ADC12VRSEL_0;
    // Set ADC12INCH (select channel A10)

    // Turn on ENC (Enable Conversion) bit at the end of the configuration
    ADC12CTL0 |= ADC12ENC;
    return;
    }



//    // X-axis: A10/P9.2, for A10 (P9DIR=x, P9SEL1=1, P9SEL0=1)
//    P9SEL1 |= BIT2;
//    P9SEL0 |= BIT2;
////Y-axis: A4/P8.7, for A4(P8DIR=x P8SEL1/0=1)
//     P8SEL0 |= BIT7;
//     P8SEL1 |= BIT7;



