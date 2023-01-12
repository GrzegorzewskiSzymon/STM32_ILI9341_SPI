#include <stdint.h>
#include "stm32g431xx.h"
#include "Libraries/RegistersConfig/RegistersConfig.h"
#include "Libraries/TFT_ILI9341/TFT_ILI9341.h"
#include "Libraries/Img_guitar_240x240.h"
#include "Libraries/TouchScreen/XPT2046.h"


int main(void)
{
	ClockFrequency_Setup();//170MHz
	GPIOA_Setup();
	Spi_ILI9341_Setup();
	Spi_XPT2046_Setup();
	Interrupt_Setup();
	Systick_Setup();

	SPI_ILI9341_ENABLE;

	ILI9341_Init();
	ILI9341_DrawPixel(0, 0, 240, 320, ILI9341_CYAN);
	ILI9341_DrawImg(0, 0, 240, 240, Img_guitar);

	while(1)
	{
	}
}


