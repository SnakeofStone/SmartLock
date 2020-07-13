//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*!
	\file		UART.h
	\author
	\date		October 24th, 2019
	\brief		Function definition of the functions of the UART driver
*/
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifndef	__UART_H__
#define	__UART_H__

    //--------------------------------------------------------------------------
    // Defines
    //--------------------------------------------------------------------------
	/*
		\def	BLUETOOTH_INTERRUPT_ENABLE
		\brief	If defined, the interrupt functionality of the driver will be enabled
	*/
	#define BLUETOOTH_INTERRUPT_ENABLE
//	#undef BLUETOOTH_INTERRUPT_ENABLE

	/*!
		\def	DEBUG_MODE_ENABLE
		\brief	If defined, the debugging functionality of the driver will be enabled
	*/
	#define DEBUG_MODE_ENABLE
//	#undef	DEBUG_MODE_ENABLE

    //--------------------------------------------------------------------------
    // Functions
    //--------------------------------------------------------------------------
	void UART_vfnDriverInit(void);

#ifdef BLUETOOTH_INTERRUPT_ENABLE
	void UART_vfnCallbackReg(void (*ptr)(void));

	void LPUART0_DriverIRQHandler();
#endif

	uint8_t UART_bfnRead(uint8_t *readVal);

	uint8_t UART_bfnSend(uint8_t *sendVal);

//------------------------------------------------------------------------------
#endif
