#include <stdint.h>
#include "stm32g431xx.h"
#include "Libraries/RegistersConfig/RegistersConfig.h"
#include  "Libraries/SPI/Spi.h"


int main(void)
{
	ClockFrequency_Setup();//170MHz
	GPIOA_Setup();
	Spi1_Setup();

	SPI1_ENABLE;

	Spi1_Send(74); //oscilloscope test

	while(1)
	{
	}
}


