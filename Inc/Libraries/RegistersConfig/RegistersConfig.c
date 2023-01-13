/*
 * RegistersConfig.c
 *
 *  Created on: Dec 28, 2022
 *      Author: Szymon Grzegorzewski
 */

#include <stdint.h>
#include "stm32g431xx.h"
#include "RegistersConfig.h"


//
// GPIOx
//

void GPIOA_Setup()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;


	//
	//SPI 1 - ILI9341
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

	//PA8 as TFT_RESET
	GPIOA->MODER &= ~(GPIO_MODER_MODER8_1);//General purpose output mode

	//PA9 as D/C
	GPIOA->MODER &= ~(GPIO_MODER_MODER9_1);//General purpose output mode


	//
	//SPI 2 - XPT2046
	//

	//PB13 as SCK

	//PA10 as MISO
	GPIOA->MODER  &=   ~(GPIO_MODER_MODE10_0);  //Alternate function mode
	GPIOA->AFR[1] |= (5<<GPIO_AFRH_AFSEL10_Pos);//AFSEL10[0101] -> AF5

	//PA11 as MOSI
	GPIOA->MODER  &=   ~(GPIO_MODER_MODE11_0);
	GPIOA->AFR[1] |= (5<<GPIO_AFRH_AFSEL11_Pos);

	//PB3 as IRQ (XPT2046)

}

void GPIOB_Setup()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	//
	//SPI 2 - XPT2046
	//

	//PB13 as SCK
	GPIOB->MODER  &=   ~(GPIO_MODER_MODE13_0);  //Alternate function mode
	GPIOB->AFR[1] |= (5<<GPIO_AFRH_AFSEL13_Pos);//AFSEL10[0101] -> AF5

	//PA10 as MISO

	//PA11 as MOSI

	//PB3 as IRQ (XPT2046)
	GPIOB->MODER  &=   ~( (GPIO_MODER_MODE3_0) | (GPIO_MODER_MODE3_1) );
}



//
// SysClk
//

void ClockFrequency_Setup()
{

	RCC->CR |= (1<<8); //HSION

	while(!(RCC->CR & (1<<10))); //HSIRDY

	RCC->CR = 0x00000063;//reset

	//Enable HSE and wait till its ready
	RCC->CR |= (1<<16); //HSEON
	while (!(RCC->CR & (1<<17)));//HSERDY


	//Select the Range 1 boost mode by clearing the R1MODE bit is in the PWR_CR5 register
	PWR->CR5 &= ~(1<<8);

	//Voltage scaling range selection
	PWR->CR1 |= (1<<9);//VOS[0;1] -> Range 1


	FLASH->ACR |= (1<<8);//Enable prefetch to mitigate the disparity in speed at higher clock rates
	FLASH->ACR |= (5<<0);//Period to the Flash access time


	                        //PLLR = 2 by default
	RCC->PLLCFGR |= (85<<8);//PLLN = 85
	RCC->PLLCFGR |= (5<<4); //PLLM = 6
	RCC->PLLCFGR |= (3<<0); //PLL SRC
	RCC->PLLCFGR |= (1<<24);//Main PLL clock selected

	//Enable the PLL again by setting PLLON
	RCC->CR |= (1<<24);

	//Wait until PLLRDY is set
	while(!(RCC->CR & 1<<25))
	{
	}

	//The system clock must be divided by 2 using the AHB prescaler before switching
	// to a higher system frequency
	RCC->CFGR |= (1<<7);

	// Configure and switch to PLL for a new system frequency
	RCC->CFGR |= (3<<0);

	//Wait for at least 1us and then reconfigure the AHB prescaler to get the needed
	// HCLK clock frequency.
	while(!(RCC->CFGR & (2<<2)) && !(RCC->CFGR & (1<<2)))
	{
	}

	RCC->CFGR &= ~(1<<7);
}

//
// SPI 1
//

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


//
// SPI 2
//

void Spi2_Setup()
{
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;
										//MSB first (by default)
										//Clock polarity to 0 when idle (by default)
										//The first clock transition is the first data capture edge (by default)
										//Data size 8-bit (by default)
										//Motorola frame format (by default)
										//No NSS pulse (by default)
	SPI2->CR1 |= SPI_CR1_MSTR;			//Master configuration
	SPI2->CR1 |= (5<<SPI_CR1_BR_Pos);	//fPCLK/64 = ~2,65Mhz
	SPI2->CR1 |= (1<<8) | (1<<9);  		//Software Slave Management
	SPI2->CR2 = 0;
}

void Spi2_Send(uint8_t *byte, uint32_t length)
{

    while (length > 0U)
    {
    	//not sure if necessary
    	if (((SPI2->SR)&(1<<1)))//Wait for TXE bit to set -> This will indicate that the buffer is empty
    	{
    		*((volatile uint8_t *) &SPI2->DR) = (*byte);//Load the data into the Data Register
    		byte++;
    		length--;
    	}

    }

    //not sure if necessary
	//Wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
	while (((SPI2->SR)&(1<<7))) {};
}

//
// Systick
//

void Systick_Setup()
{
	SysTick->LOAD = (uint32_t)170000;              //The value which will be decrementing, 24bit value
	SysTick->VAL = 0;                              //(undefined on reset)
 	SysTick->CTRL  =  (SysTick_CTRL_CLKSOURCE_Msk) //Processor clock (AHB)
 				   |  (SysTick_CTRL_ENABLE_Msk)    //Enables the counter
 				   |  (SysTick_CTRL_TICKINT_Msk);  //Exception request
}

uint64_t ms;
void SysTick_Handler()
{
	ms++;
}

//
// IRQs
//

void Interrupt_Setup()
{
	NVIC_EnableIRQ(SysTick_IRQn); //Enable interrupt from Systick
	NVIC_EnableIRQ(EXTI3_IRQn);
}

void EXTI3_Setup()
{
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PB; // Select PB3 as EXTI3
	EXTI->IMR1 |= EXTI_IMR1_IM3; // Interrupt request is not masked
	EXTI->RTSR1 |= EXTI_RTSR1_RT3; // Rising edge trigger enabled
}

void EXTI3_IRQHandler()
{
	//Do some stuff

	EXTI->PR1 |= EXTI_PR1_PIF3;
}

