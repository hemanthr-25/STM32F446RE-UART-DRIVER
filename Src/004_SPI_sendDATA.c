/*
 * 004_SPI_sendDATA.c
 *
 *  Created on: Apr 30, 2025
 *      Author: DELL
 */

//PB15 ----> MOSI
//PB14 ----> MISO
//PB9  ----> NSS
//PB13 ----> SCK PB10
//ALTERNATE FUNCTION ----> AF5

//PA5-sck sp1
//PA7-mosi

#include <stdint.h>
#include <string.h>
#include <stm32f446.h>

void delay(){
	for(uint32_t i=0; i<500000;i++);
}


void SPI2_GPIO_Inits(void){
		GPIO_Handle_t GPIOspi;

		GPIOspi.pGPIOx = GPIOB;
		GPIOspi.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_ALTFN;
		GPIOspi.GPIO_PinConfig.GPIO_PinAltFunMode= 5;
		GPIOspi.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_TYPE_PP;
		GPIOspi.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
		GPIOspi.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_NO_PUPD;

		//SCK
		GPIOspi.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_13;
		GPIO_Init(&GPIOspi);

		/*NSS
		GPIOspi.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_9;
		GPIO_Init(&GPIOspi);

		//MISO
		GPIOspi.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_6;
		GPIO_Init(&GPIOspi);
*/

		//MOSI
		GPIOspi.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_15;
		GPIO_Init(&GPIOspi);
}

SPI_Handle_t SPI2Handle;

void SPI2_Inits(void){
	SPI2Handle.pSPIx=SPI2;
	SPI2Handle.SPIConfig.SPI_BusConfig=SPI_BUS_CONFIG_FD;
	SPI2Handle.SPIConfig.SPI_CPHA=SPI_CPHA_LOW;
	SPI2Handle.SPIConfig.SPI_CPOL=SPI_CPOL_LOW;
	SPI2Handle.SPIConfig.SPI_DFF=SPI_DFF_8BITS;
	SPI2Handle.SPIConfig.SPI_DeviceMode=SPI_DEVICE_MODE_MASTER;
	SPI2Handle.SPIConfig.SPI_SSM=SPI_SSM_EN;
	SPI2Handle.SPIConfig.SPI_SclkSpeed=SPI_SCLK_SPEED_DIV16;
	SPI_Init(&SPI2Handle);
}



int main(){
	char msg[]="abcd";
	SPI2_GPIO_Inits();
	SPI2_Inits();
	SPI_SSIConfig(SPI2Handle.pSPIx, ENABLE);
	SPI_PeripheralControl(SPI2Handle.pSPIx, ENABLE);
	SPI_SendData(SPI2Handle.pSPIx,(uint8_t *)msg,strlen(msg));
	//delay();
	while(SPI_GetFlagStatus(SPI2Handle.pSPIx,SPI_BUSY_FLAG));
	SPI_PeripheralControl(SPI2Handle.pSPIx, DISABLE);
	while(1);
}
