/*
 * RegistersConfig.h
 *
 *  Created on: Dec 28, 2022
 *      Author: Szymon Grzegorzewski
 */

#ifndef LIBRARIES_REGISTERSCONFIG_REGISTERSCONFIG_H_
#define LIBRARIES_REGISTERSCONFIG_REGISTERSCONFIG_H_


void GPIOA_Setup();
void ClockFrequency_Setup();
void Systick_Setup();
void Interrupt_Setup();

extern uint64_t ms;//reference time counted from the "beginning"

#endif /* LIBRARIES_REGISTERSCONFIG_REGISTERSCONFIG_H_ */
