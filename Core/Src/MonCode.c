/*
 * MonCode.c
 *
 *  Created on: Apr 23, 2020
 *      Author: pwi
 */

#include "dac.h"
#include "adc.h"
//#incldue "stm32f4xx_hal_dac.h"

void Test(void){
	HAL_ADC_Start(&hadc1);
	HAL_DAC_Start(&hdac, DAC_CHANNEL_2);



	for (int i = 1 ; i<11;i++){
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2,DAC_ALIGN_12B_R, (370*i) );
		HAL_ADC_PollForConversion(&hadc1, 0);
		printf("%d DAC= %lu\tADC= %lu\r\n", i, HAL_DAC_GetValue(&hdac, DAC_CHANNEL_2), HAL_ADC_GetValue(&hadc1));
		vTaskDelay(1000);
	}

	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 4095);
	printf("DAC= %u\r\n", HAL_DAC_GetValue(&hdac, DAC_CHANNEL_2));
	vTaskDelay(1000);


	HAL_DAC_Stop(&hdac, DAC_CHANNEL_2);
	HAL_ADC_Stop(&hadc1);
	printf("ByeBye\n");
	vTaskDelete(NULL);
}


void test2(void){




}
