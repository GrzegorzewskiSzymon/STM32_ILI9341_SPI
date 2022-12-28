#include <stdint.h>
#include "stm32g431xx.h"
#include "Libraries/Sysclk/sysclk.h"
#include  "Libraries/SPI/Spi.h"

#define PA5_LOW  GPIOA->BSRR = 1<<21
#define PA5_HIGH GPIOA->BSRR = 1<<5

void GPIOA_Setup();

int main(void)
{
	ClockFrequency_Setup();//170MHz
	GPIOA_Setup();
	Spi1_Setup();

	SPI1_ENABLE;

	Spi1_Send(74);

	while(1)
	{
	}
}

void GPIOA_Setup()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;



	//
	//SPI 1
	//

	//PA5 as SCK
	GPIOA->MODER  &=   ~(1<<10); //Alternate function mode
	GPIOA->AFR[0] |= (5<<20);//AFSEL5[0101] -> AF5
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_0;//Medium Speed

	//PA6 as MISO
	GPIOA->MODER  &=   ~(GPIO_MODER_MODER6_0);
	GPIOA->AFR[0] |= (5<<GPIO_AFRL_AFSEL6_Pos);

	//PA7 as MOSI
	GPIOA->MODER  &=   ~(GPIO_MODER_MODER7_0);
	GPIOA->AFR[0] |= (5<<GPIO_AFRL_AFSEL7_Pos);

	//PA9 as D/C
	GPIOA->MODER &= ~(GPIO_MODER_MODER9_1);//General purpose output mode

}
