/*
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
//------------------------------------------------------------------------------
/*!
	\file   	SmartLock.c
	\date		November 27th, 2019
	\brief		Main application control
*/
//------------------------------------------------------------------------------
#include <stdio.h>
#include "MKL27Z644.h"
#include "fsl_debug_console.h"

#include "Control.h"
#include "Indicators.h"
#include "Password.h"

//------------------------------------------------------------------------------
// Local Defines
//------------------------------------------------------------------------------
/*!
    \def		TRUE
    \brief		Assigns the value of 1 to the TRUE keyword.
*/
#define			TRUE		1

/*!
    \def		FALSE
    \brief		Assigns the value of 0 to the FALSE keyword.
*/
#define 		FALSE 		0

//------------------------------------------------------------------------------
// Enums
//------------------------------------------------------------------------------
/*!
    \enum			eStateMachine
    \brief			All posible states of the state machine
*/
enum eStateMachine
{
	eSTATE_ZERO,
	eSTATE_ONE_CORRECT,
	eSTATE_ONE_WRONG,
	eSTATE_TWO_CORRECT,
	eSTATE_TWO_LOCKDOWN_ON,
	eSTATE_TWO_LOCKDOWN_OFF
};

//------------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------------
/*!
    \var		isPasswordCorrect
    \brief		Flag to show if the password introduced is correct or not.
*/
static uint8_t isPasswordCorrect = 0;

/*!
    \var		inLockdown
    \brief		Flag to show if the system is in lockdown.
*/
static uint8_t inLockdown = 0;

/*!
 	 \var		numErrors
 	 \brief		Variable to count how many errors have passed
 */
static uint8_t numErrors = 0;

//------------------------------------------------------------------------------
// Local Functions prototypes
//------------------------------------------------------------------------------
void vfnStateZero (uint8_t *stateVar);
void vfnStateOneCorrect (uint8_t *stateVar);
void vfnStateOneWrong (uint8_t *stateVar);
void vfnStateTwoCorrect (uint8_t *stateVar);
void vfnStateTwoLockdownOn (uint8_t *stateVar);
void vfnStateTwoLockdownOff (uint8_t *stateVar);

/*!
 	 \var		fnPtrArr
 	 \brief		Defines the function pointer array to the functions of the states
 */
void (*fnPtrArr[])(uint8_t *stateVar) = {
		 vfnStateZero,
		 vfnStateOneCorrect,
		 vfnStateOneWrong,
		 vfnStateTwoCorrect,
		 vfnStateTwoLockdownOn,
		 vfnStateTwoLockdownOff
};

/*!
 	 \fn		in main(void)
 	 \return	Returns 0
 	 \brief		Where the program starts executing and the state machine is
 */
int main(void)
{
  	/* Init board hardware. */
	Password_vfnDriverInit ();
	Indicators_vfnDriverInit ();
	Control_vfnDriverInit ();

	/* Define the state variable */
	static uint8_t _StateVariable = eSTATE_ZERO;

    /* Enter an infinite loop */
    while(1)
    {
    	(*fnPtrArr[_StateVariable])(&_StateVariable);
    }
    return 0 ;
}

/*!
 	 \fn		void vfnStateZero (uint8_t *stateVar)
 	 \param		stateVar Receives in which state the state machine currently is.
 	 \brief		The initial function and default state of the state machine.
				It determines what to do if the introduced pin was correct or
				not. If it was, changes to eSTATE_ONE_CORRECT state; else, changes to
				eSTATE_ONE_WRONG state.
 */
void vfnStateZero (uint8_t *stateVar)
{
	isPasswordCorrect = Password_bfnIsCorrect ();
	if (isPasswordCorrect)
	{
		*stateVar =  eSTATE_ONE_CORRECT;
	}
	else
	{
		*stateVar = eSTATE_ONE_WRONG;
	}
}

/*!
 	 \fn		void vfnStateOneCorrect (uint8_t *stateVar)
 	 \param		stateVar Receives in which state the state machine currently is.
 	 \brief		This state is called when the introduced pin is correct. This
 	  	  	  	calls Indicators_bfnCorrectPin() function to blink the green LED
 	  	  	  	and play a high-pitched tone. Then changes the state to
 	  	  	  	eSTATE_TWO_CORRECT.
 */
void vfnStateOneCorrect (uint8_t *stateVar)
{
	numErrors = 0;
	Indicators_bfnCorrectPin ();

#if 0
	if (inLockdown)
	{
		*stateVar = eSTATE_TWO_LOCKDOWN_OFF;
	}
	else
	{
		*stateVar = eSTATE_TWO_CORRECT;
	}
#endif
	*stateVar = eSTATE_TWO_CORRECT;
}

/*!
 	 \fn		void vfnStateOneWrong (uint8_t *stateVar)
 	 \param		stateVar Receives in which state the state machine currently is.
 	 \brief		This state is called when the introduced pin is incorrect. This
 	  	  	  	calls Indicators_bfnWrongPin() function to blink the red LED
 	  	  	  	and play a low-pitched tone. Then changes the state to
 	  	  	  	eSTATE_ZERO.
 */
void vfnStateOneWrong (uint8_t *stateVar)
{
	numErrors++;
	Indicators_bfnWrongPin ();
#if 0
	if (numErrors >= 3)
	{
		*stateVar = eSTATE_TWO_LOCKDOWN_ON;
	}
	else
	{
		*stateVar = eSTATE_ZERO;
	}
#endif
	*stateVar = eSTATE_ZERO;
}

/*!
 	 \fn		void vfnStateTwoCorrect (uint8_t *stateVar)
 	 \param		stateVar Receives in which state the state machine currently is.
 	 \brief		This state function is called after the void vfnStateOneWrong (uint8_t *stateVar)
 	 	 	 	function is executed. This calls the Control_bfnCorrectPin () to
 	 	 	 	activate the solenoid. Then changes the state to eSTATE_ZERO.
 */
void vfnStateTwoCorrect (uint8_t *stateVar)
{
	Control_bfnCorrectPin ();
	*stateVar = eSTATE_ZERO;
}

/*!
 	 \fn		void vfnStateTwoLockdownOn (uint8_t *stateVar)
 	 \param		stateVar Receives in which state the state machine currently is.
 	 \brief		This state function is called after the vfnStateOneWrong (uint8_t *stateVar)
 	 	 	 	function is called. This calls Control_bfnLockdownOn () and engages the window
 	 	 	 	pin to block it from opening. Then changes the state variable to eSTATE_ZERO.
 */
void vfnStateTwoLockdownOn (uint8_t *stateVar)
{
	Control_bfnLockdownOn ();
	*stateVar = eSTATE_ZERO;
}

/*!
 	 \fn		void vfnStateTwoLockdownOff (uint8_t *stateVar)
 	 \param		stateVar Receives in which state the state machine currently is.
 	 \brief		This state function is called after the vfnStateOneCorrect (uint8_t *stateVar)
 	 	 	 	function is called. This calls Control_bfnLockdownOff () and disengages the window
 	 	 	 	pin to allow it to open. Then changes the state variable to eSTATE_TWO_CORRECT.
 */
void vfnStateTwoLockdownOff (uint8_t *stateVar)
{
	Control_bfnLockdownOff ();
	*stateVar = eSTATE_TWO_CORRECT;
}
