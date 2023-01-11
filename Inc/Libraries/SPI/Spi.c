/*
 * Spi.c
 *
 *  Created on: Dec 28, 2022
 *      Author: Szymon Grzegorzewski
 */
#include <stdint.h>
#include "stm32g431xx.h"
#include "../RegistersConfig/RegistersConfig.h"

void Spi_ILI9341_Setup()
{
	Spi1_Setup();
}

void Spi_ILI9341_Send(uint8_t *byte, uint32_t length)
{
	Spi1_Send(byte, length);
}
