//------------------------------------------------------------------------------
/*!
	\file		Indicators.c
	\date		November 25th, 2019
	\brief		Function implementation for the manipulation of the LEDs
				and buzzer indicators.
*/
//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "MKL27Z644.h"
#include "Indicators.h"
#include "GPIO.h"
#include "PWM.h"
#include "serviceLayer.h"

//--------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------
/*!
    \fn				void Indicators_vfnDriverInit ()
    \brief			Initializes the required pins and ports for the LEDs and buzzer
*/
void Indicators_vfnDriverInit ()
{
	// Initialize LEDs pins and PWM pin
	// Green LED
	GPIO_vfnPortInit(ePORTA, ePIN1, eOUTPUT);
	// Red LED
	GPIO_vfnPortInit(ePORTA, ePIN2, eOUTPUT);
	// PTB18 as PWM output
	PWM_vfnDriverInit();
}

/*!
    \fn				uint8_t Indicators_bfnCorrectPin ()
    \return			Returns 1 when the process is finished.
    \brief			When called, this function changes the MOD value
    				for the PWM signal, increasing it's period, thus
    				increasing the pitch of the tone played by it.
    				Then makes the green LED blink.
*/
uint8_t Indicators_bfnCorrectPin ()
{
	uint8_t i = 0;
	PWM_bfnChangeCounter (500);
	PWM_vfnToggleSignal ();
	for (i = 0; i < 3; i++)
	{
		GPIO_bfnSetData(ePORTA, ePIN1);
		delay(200000);

		GPIO_bfnClearData(ePORTA, ePIN1);
		delay(200000);
	}
	PWM_vfnToggleSignal ();
	return 1;
}

/*!
    \fn				uint8_t Indicators_bfnWrongPin ()
    \return			Returns 1 when the process is finished.
    \brief			When called, this function changes the MOD value
    				for the PWM signal, decreasing it's period, thus
    				decreasing the pitch of the tone played by it.
    				Then makes the red LED blink.
*/
uint8_t Indicators_bfnWrongPin ()
{
	uint8_t i = 0;
	PWM_bfnChangeCounter (1000);
	PWM_vfnToggleSignal ();
	for (i = 0; i < 3; i++)
	{
		GPIO_bfnSetData(ePORTA, ePIN2);
		delay(200000);

		GPIO_bfnClearData(ePORTA, ePIN2);
		delay(200000);
	}
	PWM_vfnToggleSignal ();
	return 1;
}
