//------------------------------------------------------------------------------
/*!
	\file		Indicators.h
	\date		November 25th, 2019
	\brief		Function declaration for the manipulation of the LEDs
				and buzzer indicators.
*/
//------------------------------------------------------------------------------
#ifndef _2_HIL_INDICATORS_H_
#define _2_HIL_INDICATORS_H_

//--------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------
void Indicators_vfnDriverInit (void);

uint8_t Indicators_bfnCorrectPin (void);

uint8_t Indicators_bfnWrongPin (void);

#endif /* 2_HIL_INDICATORS_H_ */
