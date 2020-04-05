//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*!
	\file		Password.h
	\date		November 20th, 2019
	\brief		Password function declaration for reading the pressed keys in the 4x3 keypad
*/

#ifndef PASSWORD_H_
#define PASSWORD_H_
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "GPIO.h"
#include "UART.h"

//--------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------
void Password_vfnDriverInit (void);

uint8_t Password_bfnIsCorrect (void);

void Matrix_vfnPortInit (void);

uint8_t Matrix_bfnGetChar(void);

uint8_t Matrix_bfnMatrixRead (uint8_t *row, uint8_t *column);

uint8_t Matrix_bfnPorts (IO io, uint8_t iteration, uint8_t onOff);

#ifdef BLUETOOTH_INTERRUPT_ENABLE
void readVal();
#endif

#endif /* PASSWORD_H_ */
