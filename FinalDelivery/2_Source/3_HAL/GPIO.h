//------------------------------------------------------------------------------
/*!
	\file	GPIO.h
	\date	November 15th, 2019
	\brief	Function declaration of the GPIO driver
*/
//------------------------------------------------------------------------------
#ifndef GPIO_H_
#define GPIO_H_
//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "MKL27Z644.h"

//--------------------------------------------------------------------------
// Enums
//--------------------------------------------------------------------------
/*!
	\enum	PORTS
	\brief	Enum declaration for the PORTS to enable in SCGC5
*/
typedef enum
{
	ePORTA,
	ePORTB,
	ePORTC,
	ePORTD,
	ePORTE
} PORTS;

/*!
	\enum	PINS
	\brief	Enum declaration for the available PINS in the PCR
*/
typedef enum
{
	ePIN0, ePIN1, ePIN2, ePIN3, ePIN4,
	ePIN5, ePIN6, ePIN7, ePIN8, ePIN9,
	ePIN10, ePIN11, ePIN12, ePIN13, ePIN14,
	ePIN15, ePIN16, ePIN17, ePIN18, ePIN19,
	ePIN20, ePIN21, ePIN22, ePIN23, ePIN24,
	ePIN25, ePIN26, ePIN27, ePIN28, ePIN29,
	ePIN30, ePIN31
} PINS;

/*!
	\enum	IO
	\brief	Enum declaration to declare a pin as INPUT or OUTPUT
*/
typedef enum
{
	eINPUT,
	eOUTPUT
} IO;


//--------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------
void GPIO_vfnPortInit(PORTS port, PINS pin, IO io);

uint8_t GPIO_bfnSetData(PORTS port, PINS pin);

uint8_t GPIO_bfnReadData(PORTS port, PINS pin, uint8_t *value);

uint8_t GPIO_bfnClearData(PORTS port, PINS pin);

uint8_t GPIO_bfnToggleData(PORTS port, PINS pin);

uint8_t GPIO_bfnData(PORTS port, PINS pin, uint8_t *value);

#endif /* GPIO_H_ */
