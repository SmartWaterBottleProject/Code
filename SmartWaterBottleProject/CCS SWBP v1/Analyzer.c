/*
 * Analyze.c
 *
 *  Created on: Aug 23, 2021
 *      Author: User
 */
#include <Ports.h>
#include <Analyzer.h>

// Configure PWM Move motor
//Enable and disable LD
//Move Motor...

//Once finished call exporter to send data to bluetooth module

void Analyze(void){

    P9OUT ^= GPIO_PIN7; // Toggle the green LED

    return;
}
