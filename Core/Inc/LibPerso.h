/*
 * LibPerso.h
 *
 *  Created on: Mar 25, 2020
 *      Author: pwi
 */

#ifndef INC_LIBPERSO_H_
#define INC_LIBPERSO_H_
#include "FreeRTOS.h"
#include "semphr.h"

extern SemaphoreHandle_t MonSem, MonSemUART;
extern char prompt[2];

#endif /* INC_LIBPERSO_H_ */
