//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*!
	\file		PWM.h
	\date		October 17th, 2019
	\brief		Function declaration for the PWM driver used to generate a
				square signal with a fixed period
*/
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifndef _3_HAL_PWM_H_
#define _3_HAL_PWM_H_
	//--------------------------------------------------------------------------
    // Functions
    //--------------------------------------------------------------------------
	/*!
		\fn 		void PWM_vfnDriverInit()
		\return		No return
		\brief		Driver initialization
	*/
	void PWM_vfnDriverInit();

	/*!
		\fn 		uint8_t PWM_bfnAngleAdjustment (uint8_t bNewAngle)
		\return		Returns 0 if the angle modification was not successful; returns 1 if it was
		\brief		Adjust the signal angle
	*/
	uint8_t PWM_bfnAngleAdjustment (uint8_t bNewAngle);

	/*!
		\fn 		uint8_t PWM_bInitialPosition (void)
		\return		Returns 0 if the angle modification was not successful; returns 1 if it was
		\brief		Set the servo motor to the initial position
	*/
	uint8_t PWM_bInitialPosition (void);

	/*!
	 	 \fn		uint8_t PWM_bfnChangeCounter (uint16_t counter)
	 	 \param		counter Number which will be the new mod value
	 	 \return
	 	 \brief
	 */
	uint8_t PWM_bfnChangeCounter (uint16_t counter);

	void PWM_vfnToggleSignal (void);

//------------------------------------------------------------------------------
#endif /* _3_HAL_PWM_H_ */
