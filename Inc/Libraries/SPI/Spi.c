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

void Spi1_Send(uint8_t byte)
{

	while (!((SPI1->SR)&(1<<1))) {};  // wait for TXE bit to set -> This will indicate that the buffer is empty
	SPI1->DR = byte;  // load the data into the Data Register

	while (((SPI1->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication

	//  Clear the Overrun flag by reading DR and SR
	uint8_t temp = SPI1->DR;
			temp = SPI1->SR;

}
