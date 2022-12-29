/*
 * Spi.c
 *
 *  Created on: Dec 28, 2022
 *      Author: Szymon Grzegorzewski
 */
#include <stdint.h>
#include "stm32g431xx.h"

void Spi1_Setup()
{
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
										//MSB first (by default)
										//Clock polarity to 0 when idle (by default)
										//The first clock transition is the first data capture edge (by default)
										//Data size 8-bit (by default)
										//Motorola frame format (by default)
										//No NSS pulse (by default)
	SPI1->CR1 |= SPI_CR1_MSTR;			//Master configuration
	SPI1->CR1 |= (4<<SPI_CR1_BR_Pos);	//fPCLK/32 = ~5,3Mhz
	SPI1->CR1 |= (1<<8) | (1<<9);  		//Software Slave Management
	SPI1->CR2 = 0;
}

void Spi1_Send(uint8_t *byte, uint32_t length)
{

    while (length > 0U)
    {
    	//not sure if necessary
    	if (((SPI1->SR)&(1<<1)))//Wait for TXE bit to set -> This will indicate that the buffer is empty
    	{
    		*((volatile uint8_t *) &SPI1->DR) = (*byte);//Load the data into the Data Register
    		byte++;
    		length--;
    	}

    }

    //not sure if necessary
	//Wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
	while (((SPI1->SR)&(1<<7))) {};

}
