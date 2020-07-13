//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*!
	\file		PWM.c
	\date		October 17th, 2019
	\brief		Function implementation for the PWM driver used to generate a
				square signal with a fixed period
*/
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "MKL27Z644.h"
#include "PWM.h"

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
/*!
    \def		MCGIRCLK_ALT
    \brief		Macro for selecting the MCGIRCLK clock option in TPMSRC
    			clock select option
*/
#define		MCGIRCLK_ALT		3

/*!
    \def	TPM_MOD
    \brief	1001 is the value for a 2ms signal
*/
#define 	TPM_MOD 			1001

/*!
    \def	PWM_PIN
    \brief	Pin number used for the PWM signal
*/
#define 	PWM_PIN 			18

/*!
    \def	MUX_AS_PWM
    \brief	Select the pin mode as ALT3 for the PWM signal output
*/
#define 	MUX_AS_PWM 			(3<<8)

/*!
    \def	COUNTER_RESET
    \brief	Reset value for the counter
*/
#define 	COUNTER_RESET 		0

/*!
    \def	DBGMODE_MASK
    \brief	Mask to activate the debug mode
*/
#define 	DBGMODE_MASK 		(3<<6)

/*!
    \def	PS_8
    \brief	Prescaler factor option value
*/
#define 	PS_8 				3

/*!
    \def	CMOD
    \brief
*/
#define 	CMOD 				1

/*!
    \def	CHANNEL
    \brief	Channel value
*/
#define 	CHANNEL 			0

/*!
    \def	TPM_CENTER_ALIGN
    \brief	Select the mode signal output
*/
#define 	TPM_CENTER_ALIGN 	0x28		// 0010 1000

/*!
 	 \def	TPM_MINOFFSIDE
 	 \brief	Minimum offside for a 1ms up-time square square signal
 */
#define 	TPM_MINOFFSIDE 		(TPM_MOD>>1)

/*!
 	 \def	HALF_WORD
 	 \brief	Half the size of the microprocessor word, which is 32 bits
 */
#define 	HALF_WORD			0xFFFF

//------------------------------------------------------------------------------
/*!
    \fn			void PWM_vfnDriverInit()
    \brief		Initialize the TPM0 module and configure the PWM signal to output a 20ms square wave and a 1ms up-time
*/
void PWM_vfnDriverInit()
{
	// Clock enable of the peripherals in SIM
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(MCGIRCLK_ALT);

	PORTB->PCR[PWM_PIN] |= MUX_AS_PWM;					// Set PORTB pin 18 as output for the PWM

	TPM2->CNT = 0;										// Reset the counter to zero 0
	TPM2->MOD = TPM_MOD;								// Value for the modulus operation of the counter

	TPM2->CONF = DBGMODE_MASK;							// Set debug mode to increment the counter

	TPM2->SC |= PS_8;									// Set prescale factor to divide by 8
	TPM2->SC |= TPM_SC_CPWMS_MASK;						// Set CPWMS to up-down counter
//	TPM2->SC |= CMOD << TPM_SC_CMOD_SHIFT;				// Set counter increment once per TPM clock

	TPM2->CONTROLS[CHANNEL].CnSC = TPM_CENTER_ALIGN;	// Configure the output signal as shown in the configuration table
	TPM2->CONTROLS[CHANNEL].CnV = TPM_MINOFFSIDE;		// Set the initial value for the offside for a custom duty cycle signal
}

/*!
	\fn			uint8_t PWM_bfnAngleAdjustment (uint8_t bNewAngle)
	\param		bNewAngle The new angle to position the servo to is passed through this variable
	\return		Return 0 if the adjustment wasn't successful; return 1 if it was
	\brief		Adjust the angle of the servo motor by modifying the offside of the signal to have it between 1ms and 2ms
*/
uint8_t PWM_bfnAngleAdjustment (uint8_t bNewAngle)
{
	if (bNewAngle >= 0 && bNewAngle <= 180)
	{
		TPM2->CONTROLS[CHANNEL].CnV = TPM_MINOFFSIDE + (bNewAngle<<2);
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
	\fn 		uint8_t PWM_bInitialPosition (void)
	\return		Return 1 if the angle adjustment was successful; return 0 if it was not
	\brief
 */
uint8_t PWM_bInitialPosition (void)
{
	TPM2->CONTROLS[CHANNEL].CnV = TPM_MINOFFSIDE;
	return 1;
}

/*!
 	 \fn		uint8_t PWM_bfnChangeCounter(uint16_t counter)
 	 \param		counter New value for the PWM mod register
 	 \return	Returns 1 if the operation was successful; else, returns 0
 	 \brief		Turns of the PWM counter and changes the MOD value to modify the signal's period
 */

uint8_t PWM_bfnChangeCounter(uint16_t counter)
{
	// Disable stuff
//	TPM2->SC &= ~(CMOD << TPM_SC_CMOD_SHIFT);		// Stop the counter
	TPM2->CONF = 0;

	// Change the counter
	TPM2->CNT |= 0;							// Change the value of the counter
	TPM2->MOD = counter;
	TPM2->CONTROLS[CHANNEL].CnV = counter>>1;

	// Enable stuff
	TPM2->CONF = DBGMODE_MASK;
//	TPM2->SC |= CMOD << TPM_SC_CMOD_SHIFT;			// Continue the counter

	if ((TPM2->MOD & HALF_WORD) == counter)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*!
 	 \fn		void PWM_vfnToggleSignal ()
 	 \brief		Alternates the state of the PWM signal, ON or OFF
 */
void PWM_vfnToggleSignal ()
{
	TPM2->SC ^= CMOD << TPM_SC_CMOD_SHIFT;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

