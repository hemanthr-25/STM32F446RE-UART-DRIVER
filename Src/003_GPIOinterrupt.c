/*
 * 003_GPIOinterrupt.c
 *
 *  Created on: Apr 27, 2025
 *      Author: DELL
 */


//USERbutton is in PC13
//USERled is in PA5
#include <stdint.h>
#include <stm32f446.h>

void delay(){
	for(uint32_t i=0; i<500000/2;i++);
}
GPIO_Handle_t GPIOButton;
GPIO_Handle_t GPIOLed;

int main(void)
{
	//GPIO_Handle_t GPIOLed;
	GPIOLed.pGPIOx = GPIOA;
	GPIOLed.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_5;
	GPIOLed.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_OUT;
	GPIOLed.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_TYPE_PP;
	GPIOLed.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_MEDIUM;
	GPIOLed.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_NO_PUPD;

	//GPIO_Handle_t GPIOButton;
	GPIOButton.pGPIOx = GPIOC;
	GPIOButton.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_13;
	GPIOButton.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_IT_FT;
	GPIOButton.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_MEDIUM;
	GPIOButton.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_NO_PUPD;

	GPIO_Init(&GPIOLed);
	GPIO_Init(&GPIOButton);

	GPIO_IRQInterruptConfig(IRQ_NO_EXTI15_10, ENABLE);
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI15_10,NVIC_IRQ_PRI0);

    /*while(1){
    	if(GPIO_ReadFromInputPin(GPIOButton.pGPIOx,GPIO_PIN_NO_13)==1){
    	GPIO_WriteToOutputPin(GPIOLed.pGPIOx,GPIOLed.GPIO_PinConfig.GPIO_PinNumber,SET);
    	}
    	else{
    	GPIO_WriteToOutputPin(GPIOLed.pGPIOx,GPIOLed.GPIO_PinConfig.GPIO_PinNumber,RESET);
    	}
    }*/
}

void EXTI15_10_IRQHandler(){

	GPIO_IRQHandling(GPIO_PIN_NO_13);
	if(GPIO_ReadFromInputPin(GPIOLed.pGPIOx,GPIOLed.GPIO_PinConfig.GPIO_PinNumber)==1){
	GPIO_WriteToOutputPin(GPIOLed.pGPIOx,GPIOLed.GPIO_PinConfig.GPIO_PinNumber,RESET);
	}
	else{
		GPIO_WriteToOutputPin(GPIOLed.pGPIOx,GPIOLed.GPIO_PinConfig.GPIO_PinNumber,SET);
	}

}
