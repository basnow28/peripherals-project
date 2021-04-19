/*
 * stm32f4_tim10.c
 *
 *  Created on: Apr 11, 2021
 *      Author: barbara
 */


#include "stm32f4_tim2.h"
static int *interrupt_count = 0;

void TIM2_INIT(){
		// Enable clock for TIM2
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		/* TODO - Update the timing to 1ms */
	    // TIM2 initialization overflow every 500ms
	    // TIM2 by default has clock of 84MHz
	    // Here, we must set value of prescaler and period,
	    // so update event is 1000Hz or 1ms
	    // Update Event (Hz) = timer_clock / ((TIM_Prescaler + 1) *
	    // (TIM_Period + 1))
	    // Update Event (Hz) = 84MHz / ((4199 + 1) * (9999 + 1)) =
		//TODO 1000 Hz
	    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	    TIM_TimeBaseInitStruct.TIM_Prescaler = 39;
	    TIM_TimeBaseInitStruct.TIM_Period = 2099;
	    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;

	    // TIM2 initialize
	    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

	    // Nested vectored interrupt settings
	    // TIM2 interrupt is most important (PreemptionPriority and
	    // SubPriority = 0)
	    NVIC_InitTypeDef NVIC_InitStruct;
	    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStruct);

	    // Enable TIM2 interrupt
	    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	    // Start TIM2*/
	    TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)
{
	interrupt_count += 1;
    // Checks whether the TIM2 interrupt has occurred or not
    if (TIM_GetITStatus(TIM2, TIM_IT_Update))
    {
        // Toggle green LED (GPIOG13)
    	if((*interrupt_count)%200 == 0){
    		//Toggle led every 200 interrupts
    		// restart the interrupt count
    		STM_EVAL_LEDToggle(LED3);
    		interrupt_count = 0;
    	}
    	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

