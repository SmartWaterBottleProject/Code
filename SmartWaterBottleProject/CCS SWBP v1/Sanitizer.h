/*
 * Sanitizer.h
 *
 *  Created on: Sep 1, 2021
 *      Author: dgpic
 */

#ifndef SANITIZER_H_
#define SANITIZER_H_

#include <stdbool.h>


//Bool StartOrStop: 0-Call function to Start Sanitizer, 1-Call function to Stop Sanitizer
void Sanitize(bool*, bool StartOrStop);

#endif /* SANITIZER_H_ */
