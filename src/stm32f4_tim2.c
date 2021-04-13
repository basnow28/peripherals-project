/*
 * stm32f4_tim10.c
 *
 *  Created on: Apr 11, 2021
 *      Author: barbara
 */


#include "stm32f4_tim2.h"

void TIM2_INIT(){
		// Enable clock for TIM2
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, ENABLE);

		/* TODO - Update the timing to 1ms */
	    // TIM2 initialization overflow every 500ms
	    // TIM2 by default has clock of 84MHz
	    // Here, we must set value of prescaler and period,
	    // so update event is 0.5Hz or 500ms
	    // Update Event (Hz) = timer_clock / ((TIM_Prescaler + 1) *
	    // (TIM_Period + 1))
	    // Update Event (Hz) = 84MHz / ((4199 + 1) * (9999 + 1)) = 2 Hz
	    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	    TIM_TimeBaseInitStruct.TIM_Prescaler = 4199;
	    TIM_TimeBaseInitStruct.TIM_Period = 9999;
	    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;

	    // TIM2 initialize
	    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	    // Enable TIM2 interrupt
	    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	    // Start TIM2
	    TIM_Cmd(TIM2, ENABLE);

	    // Nested vectored interrupt settings
	    // TIM2 interrupt is most important (PreemptionPriority and
	    // SubPriority = 0)
	    NVIC_InitTypeDef NVIC_InitStruct;
	    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStruct);
}

void TIM2_IRQHandler()
{
    // Checks whether the TIM2 interrupt has occurred or not
    if (TIM_GetITStatus(TIM2, TIM_IT_Update))
    {
        // Toggle green LED (GPIOG13)
    	STM_EVAL_LEDToggle(LED3);
    }
}

