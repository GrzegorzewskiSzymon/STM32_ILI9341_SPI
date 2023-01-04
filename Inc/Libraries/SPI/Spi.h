/*
 * Spi.h
 *
 *  Created on: Dec 28, 2022
 *      Author: Szymon Grzegorzewski
 */

#ifndef LIBRARIES_SPI_SPI_H_
#define LIBRARIES_SPI_SPI_H_


#define SPI1_ENABLE  SPI1->CR1 |=  SPI_CR1_SPE; //SPI  enable
#define SPI1_DISABLE SPI1->CR1 &=~ SPI_CR1_SPE; //SPI disable

void Spi1_Setup();
void Spi1_Send(uint8_t *byte, uint32_t length);

#endif /* LIBRARIES_SPI_SPI_H_ */
