//------------------------------------------------------------------------------
/*!
	\file	GPIO.c
	\date	November 15th, 2019
	\brief	Function implementation of the GPIO driver
*/

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "GPIO.h"

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
/*!
    \def	DELTA_ADDR_PORT
    \brief	Memory size difference between each PORTn base address
*/
#define DELTA_ADDR_PORT		0x1000u

/*!
    \def	DELTA_ADDR_GPIO
    \brief	Memory size difference between each GPIOn base address
*/
#define DELTA_ADDR_GPIO		0x40u


/*!
 * 	\def	BASE_PORT_BIT
 * 	\brief	Base port bit register position (PORTA = bit 9)
 */
#define BASE_PORT_BIT		9

/*!
 * 	\def	PORT
 * 	\brief	Macro replacement of the custom pointer for the PORT register
 */
#define PORT				((PORT_Type *)(PORTA_BASE + (DELTA_ADDR_PORT * port)))

/*!
 * 	\def	GPIO
 * 	\brief	Macro replacement of the custom pointer for the GPIO register
 */
#define GPIO				((GPIO_Type *)(GPIOA_BASE + (DELTA_ADDR_GPIO * port)))
//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------
/*!
    \fn			void GPIO_vfnPortInit(PORTS port, PINS pin, IO io)
    \param		port	Receives the port the user wants to enable
    \param		pin		Receives the pin the user wants to enable
    \param		io		Receives the I/O configuration the user wants for the activated pin
    \brief  	Activates the indicated port and pin as INPUT or OUTPUT
*/
void GPIO_vfnPortInit(PORTS port, PINS pin, IO io)
{
	// Enable PORTn in SIM->SCGC5
	SIM->SCGC5 |= (1<<(BASE_PORT_BIT + port));

	// Define the desired port pin as GPIO
	PORT->PCR[pin] |= PORT_PCR_MUX(1);

	// Define the desired port pin as INPUT or OUTPUT
	GPIO->PDDR |= (io<<pin);
}

/*!
    \fn			uint8_t GPIO_bfnSetData(PORTS port, PINS pin)
    \param		port	Receives the port the user wants to enable
    \param		pin		Receives the pin the user wants to enable
    \return		If writing 1 to the indicated port pin was successful, returns 1; else, returns 0
    \brief		Sets the indicated port pin to 1
*/
uint8_t GPIO_bfnSetData(PORTS port, PINS pin)
{
	if (GPIO->PDDR & (1<<pin))
	{
		GPIO->PSOR |= (1<<pin);
		return 1;
	}
	else
	{
		return 0;
	}
}

/*!
    \fn			uint8_t GPIO_bfnReadData(PORTS port, PINS pin, uint8_t *value)
    \param		port	Receives the port the user wants to enable
    \param		pin		Receives the pin the user wants to enable
    \param		value	Receives a pointer to a variable where the read value will be stored
    \return		If reading the value from the port pin was successful, returns 1; else, returns 0
    \brief		Reads the indicated port pin data en stores it in value
*/
uint8_t GPIO_bfnReadData(PORTS port, PINS pin, uint8_t *value)
{
	if (!(GPIO->PDDR & (1<<pin)))
	{
		*value = !!(GPIO->PDIR & (1<<pin));
		return 1;
	}
	else
	{
		return 0;
	}
}

/*!
    \fn			uint8_t GPIO_bfnClearData(PORTS port, PINS pin)
    \param		port	Receives the port the user wants to enable
    \param		pin		Receives the pin the user wants to enable
    \return		If clearing the value from the port pin was successful, returns 1; else, returns 0
    \brief		Clears the indicated port pin data
*/
uint8_t GPIO_bfnClearData(PORTS port, PINS pin)
{
	if(GPIO->PDDR & (1<<pin))
	{
		GPIO->PCOR |= (1<<pin);
		return 1;
	}
	else
	{
		return 0;
	}
}

/*!
    \fn			uint8_t GPIO_bfnToggleData(PORTS port, PINS pin)
    \param		port	Receives the port the user wants to manipulate
    \param		pin		Receives the pin the user wants to manipulate
    \return		If toggling the value from the port pin was successful, returns 1; else, returns 0
    \brief		Toggles the indicated port pin data
*/
uint8_t GPIO_bfnToggleData(PORTS port, PINS pin)
{
	if(GPIO->PDDR & (1<<pin))
	{
		GPIO->PTOR |= (1<<pin);
		return 1;
	}
	else
	{
		return 0;
	}
}

/*!
    \fn			uint8_t GPIO_bfnData(PORTS port, PINS pin, uint8_t *value)
    \param		port	Receives the port the user wants to manipulate
    \param		pin		Receives the pin the user wants to manipulate
    \param		value	Receives a pointer to a variable from where the value will be read and written to the
    \return		If writing the value to the port pin was successful, returns 1; else, returns 0
    \brief		Writes the passed value in the PDOR register
*/
uint8_t GPIO_bfnData(PORTS port, PINS pin, uint8_t *value)
{
	if(GPIO->PDDR & (1<<pin))
	{
		if (*value)
		{
			GPIO->PDOR |= (1<<pin);
		}
		else
		{
			GPIO->PDOR &= ~(1<<pin);
		}
		return 1;
	}
	else
	{
		return 0;
	}
}
//------------------------------------------------------------------------------
