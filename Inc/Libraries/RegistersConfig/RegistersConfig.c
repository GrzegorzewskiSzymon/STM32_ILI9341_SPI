/*
 * RegistersConfig.c
 *
 *  Created on: Dec 28, 2022
 *      Author: Szymon Grzegorzewski
 */

#include <stdint.h>
#include "stm32g431xx.h"
#include "RegistersConfig.h"

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

	//PA8 as TFT_RESET
	GPIOA->MODER &= ~(GPIO_MODER_MODER8_1);//General purpose output mode

	//PA9 as D/C
	GPIOA->MODER &= ~(GPIO_MODER_MODER9_1);//General purpose output mode

}

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

void Systick_Setup()
{
	SysTick->LOAD = (uint32_t)170000;                //The value which will be decrementing, 24bit value
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

void Interrupt_Setup()
{
	NVIC_EnableIRQ(SysTick_IRQn); //Enable interrupt from Systick
}

