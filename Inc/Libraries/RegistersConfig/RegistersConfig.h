/*
 * RegistersConfig.h
 *
 *  Created on: Dec 28, 2022
 *      Author: Szymon Grzegorzewski
 */

#ifndef LIBRARIES_REGISTERSCONFIG_REGISTERSCONFIG_H_
#define LIBRARIES_REGISTERSCONFIG_REGISTERSCONFIG_H_

//
// SPI 1 - ILI9341
//

#define SPI_ILI9341_ENABLE  SPI1->CR1 |=  SPI_CR1_SPE //SPI  enable
#define SPI_ILI9341_DISABLE SPI1->CR1 &=~ SPI_CR1_SPE //SPI disable
//PA8 as ILI9341_RESET
#define ILI9341_RESET_LOW GPIOA->BSRR |= GPIO_BSRR_BR8;
#define ILI9341_RESET_HIG GPIOA->BSRR |= GPIO_BSRR_BS8;
//PA9 as D/C
#define ILI9341_DC_COMMAND GPIOA->BSRR |= GPIO_BSRR_BR9; //LOW  - Command
#define ILI9341_DC_DATA    GPIOA->BSRR |= GPIO_BSRR_BS9; //HIGH - Data


//
// SPI 2 - XPT2046
//

#define SPI_XPT2046_ENABLE  SPI2->CR1 |=  SPI_CR1_SPE //SPI  enable
#define SPI_XPT2046_DISABLE SPI2->CR1 &=~ SPI_CR1_SPE //SPI disable

//PB3
#define XPT2046_PENIRQ_IS_HIGH (GPIOB->IDR & GPIO_IDR_IDR_3)

//
// Functions
//

void GPIOA_Setup();
void GPIOB_Setup();
void ClockFrequency_Setup();
void Systick_Setup();
void Spi1_Setup();
void Spi1_Send(uint8_t *byte, uint32_t length);
void Spi2_Setup();
void Spi2_Send(uint8_t *byte, uint32_t length);
uint8_t Spi2_Receive_8b(uint8_t *data);
void Spi2_Transreceive_8b(uint8_t *dataTx, uint16_t lengthTx, uint8_t *dataRx, uint16_t lengthRx);

void EXTI3_Setup();
extern uint64_t ms;//reference time counted from the "beginning"

#endif /* LIBRARIES_REGISTERSCONFIG_REGISTERSCONFIG_H_ */
