/*
 * Spi.h
 *
 *  Created on: Dec 28, 2022
 *      Author: Szymon Grzegorzewski
 */

#ifndef LIBRARIES_SPI_SPI_H_
#define LIBRARIES_SPI_SPI_H_

void Spi_ILI9341_Setup();
void Spi_ILI9341_Send(uint8_t *byte, uint32_t length);

#endif /* LIBRARIES_SPI_SPI_H_ */
