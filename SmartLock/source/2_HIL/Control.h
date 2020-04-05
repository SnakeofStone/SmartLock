//--------------------------------------------------------------------------
/*!
	\file			Control.h
	\date			November 27th, 2019
	\brief			Function declaration for the control of the relays of
					the solenoid and lightbulb, and the H bridge of the motor
					that locks the window.
*/
//--------------------------------------------------------------------------
#ifndef _2_HIL_CONTROL_H_
#define _2_HIL_CONTROL_H_

//--------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------
void Control_vfnDriverInit (void);

uint8_t Control_bfnCorrectPin (void);

uint8_t Control_bfnLockdownOn (void);

uint8_t Control_bfnLockdownOff (void);

#endif /* 2_HIL_CONTROL_H_ */
