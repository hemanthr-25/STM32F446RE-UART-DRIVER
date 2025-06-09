/*
 * I2C_master_tx.c
 *
 *  Created on: May 11, 2025
 *      Author: DELL
 */

/*
 * PB6-> SCL
 * PB9 or PB7 -> SDA
 */

#include<stdio.h>
#include<string.h>
#include <stm32f446.h>

#define MY_ADDR 0x61
#define SLAVE_ADDR  0x68

//arduino one wire library support only 32 bit transaction at a time so NOTE the limit
uint8_t some_data[] = "Welcome to 7volts\n";

GPIO_Handle_t GPIOButton;
I2C_Handle_t I2C1Handle;

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}

void I2C1_GPIOInits(void)
{
	GPIO_Handle_t I2CPins;

	/*Note : Internal pull-up resistors are used */

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	/*
	 * Note : In the below line use GPIO_NO_PUPD option if you want to use external pullup resistors, then you have to use 3.3K pull up resistors
	 * for both SDA and SCL lines
	 */
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPins. GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCL
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&I2CPins);


	//SDA
	//Note : since we found a glitch on PB9 , you can also try with PB7
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;

	GPIO_Init(&I2CPins);


}



void GPIO_Button_Init(void){

	GPIOButton.pGPIOx = GPIOC;
	GPIOButton.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_13;
	GPIOButton.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_IN;
	GPIOButton.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_MEDIUM;
	GPIOButton.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_NO_PUPD;
	GPIO_Init(&GPIOButton);
}

void I2C1_Inits(void)
{
	I2C1Handle.pI2Cx = I2C1;
	I2C1Handle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_DeviceAddress = MY_ADDR;
	I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

	I2C_Init(&I2C1Handle);

}


int main(void)
{
		//BUTTON INIT
		GPIO_Button_Init();

		//I2C pin inits
		I2C1_GPIOInits();

		//I2C peripheral configuration
		I2C1_Inits();

		//enable the i2c peripheral
		I2C_PeripheralControl(I2C1,ENABLE);

		while(1)
			{
				//wait till button is pressed
				while( GPIO_ReadFromInputPin(GPIOC,GPIO_PIN_NO_13) );

				//to avoid button de-bouncing related issues 200ms of delay
				delay();

				//send some data to the slave
				I2C_MasterSendData(&I2C1Handle,some_data,strlen((char*)some_data),SLAVE_ADDR,0);
			}

}
