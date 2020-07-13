//------------------------------------------------------------------------------
/*!
	\file			Control.c
	\date			November 27th, 2019
	\brief			Function implementation for the control of the relays of
					the solenoid and lightbulb, and the H bridge of the motor
					that locks the window.
*/
//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "MKL27Z644.h"
#include "Control.h"
#include "GPIO.h"
#include "serviceLayer.h"

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
/*!
    \def		TRUE
    \brief		Defines a value of 1 for the TRUE keyword.
*/
#define		TRUE		1

/*!
    \def		FALSE
    \brief		Defines a value of 0 for the FALSE keyword
*/
#define		FALSE 		0

//------------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------------
/*!
    \var		inLockdown
    \brief		Stores a value which represents if the house is in lockdown or not.
*/
static uint8_t inLockdown = FALSE;

//--------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------
/*!
 	 \fn		void Control_vfnDriverInit (void)
 	 \brief		Initializes the required ports and pins for the solenoid
				and lightbulb relays, as well as both pins required to drive
				the H bridge.
 */
void Control_vfnDriverInit (void)
{
	///////////////// Control ports /////////////////
	// Motor control
	//		PORTB->PIN0		Motor forward
	// 		PORTB->PIN1		Motor backwards
	GPIO_vfnPortInit (ePORTB, ePIN0, eOUTPUT);
	GPIO_vfnPortInit (ePORTB, ePIN1, eOUTPUT);
	// Solenoid relay
	//		PORTA->PIN12	Activate relay
	GPIO_vfnPortInit (ePORTA, ePIN12, eOUTPUT);
	// Lightbulb relay
	//		PORTE->PIN25	Activate relay
	GPIO_vfnPortInit (ePORTE, ePIN25, eOUTPUT);

	// Initial values
	GPIO_bfnSetData (ePORTB, ePIN0);
	GPIO_bfnSetData (ePORTB, ePIN1);
	GPIO_bfnSetData (ePORTA, ePIN12);
}

/*!
 	 \fn		uint8_t Control_bfnCorrectPin (void)
 	 \return	Returns 1 when the process is finalized.
 	 \brief		When called, this function activates the solenoid relay and,
				if in lockdown, deactivates the window pin.
 */
uint8_t Control_bfnCorrectPin (void)
{
	// Activate the solenoid relay
	GPIO_bfnClearData (ePORTA, ePIN12);
	delay (2000000);
	GPIO_bfnSetData (ePORTA, ePIN12);

	// Unlock the window pin if enabled
	if (inLockdown)
	{
		GPIO_bfnClearData (ePORTB, ePIN0);
		delay (2000000);
		GPIO_bfnSetData (ePORTB, ePIN0);
		inLockdown = TRUE;
	}

	return 1;
}

/*!
 	 \fn		uint8_t Control_bfnLockdownOn (void)
 	 \return	Returns 1 if the house was not in lock down and activated
				it; else, returns 0.
 	 \brief		When called, if the house is in lockdown, activates the motor
				to lock the window.
 */
uint8_t Control_bfnLockdownOn (void)
{
	// Activate the motor forward
	if (!inLockdown)
	{
		GPIO_bfnClearData (ePORTB, ePIN0);
		delay (2000000);
		GPIO_bfnSetData (ePORTB, ePIN0);
		inLockdown = TRUE;

		return 1;
	}
	else
	{
		return 0;
	}
}

/*!
 	 \fn		uint8_t Control_bfnLockdownOff (void)
 	 \return	If the house was on lockdown, returns 1; else, returns 0.
 	 \brief		When called, if the house is in lockdown, it disengages
				the lock pin of the window.
 */
uint8_t Control_bfnLockdownOff (void)
{
	// Activate the motor backwards
	if (inLockdown)
	{
		GPIO_bfnClearData (ePORTB, ePIN1);
		delay (2000000);
		GPIO_bfnSetData (ePORTB, ePIN1);
		inLockdown = FALSE;

		return 1;
	}
	else
	{
		return 0;
	}
}
