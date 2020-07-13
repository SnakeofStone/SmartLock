/*!
	\file   	serviceLayer.c
	\date		November 27th, 2019
	\brief		Function implementation of the functions to be used throughout
				the code without any direct IO to the board pins
*/

typedef unsigned long uint32_t;

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "serviceLayer.h"

//--------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------
/*!
    \fn			void delay (uint32_t count)
    \param		count	Number to which the KL27z board will count from
    \brief		Function with a blocking loop to delay any process n seconds
*/
void delay (uint32_t count)
{
	while(count--)
	{
		__asm volatile("nop");
	}
}
//------------------------------------------------------------------------------


