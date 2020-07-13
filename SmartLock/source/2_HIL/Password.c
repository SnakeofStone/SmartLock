//------------------------------------------------------------------------------
/*!
	\file		Password.c
	\date		November 20th, 2019
	\brief		Password function implementation for reading the pressed keys in the 4x3 keypad
*/
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "Password.h"
#include <stdio.h>

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
/*!
    \def		ROWS
    \brief		Number of rows in the matrix keyboard
*/
#define 	ROWS		4

/*!
    \def		COLUMNS
    \brief		Number of columns in the matrix keyboard
*/
#define 	COLUMNS		3

/*!
    \def		ON
    \brief		Signals an 'ON' signal
*/
#define		ON			1

/*!
    \def		OFF
    \brief		Signals an 'OFF' signal
*/
#define		OFF			0

/*!
 * \def 		AS_CHAR
 * \brief		If this macro is defined, the values returned from the matrix
 * 				will be as ASCII chars; else, will be as integers
 */
#define AS_CHAR
//#undef AS_CHAR

//------------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------------
/*!
    \var		matrix
    \brief		Matrix for the keyboard values
*/
#ifdef AS_CHAR
static uint8_t matrix[ROWS][COLUMNS] = {
		{'1', '2', '3'},
		{'4', '5', '6'},
		{'7', '8', '9'},
		{'*', '0', '#'}
};
#else
static uint8_t matrix[ROWS][COLUMNS] = {
		{ 1, 2, 3},
		{ 4, 5, 6},
		{ 7, 8, 9},
		{10, 0, 11}
};
#endif

#ifdef BLUETOOTH_INTERRUPT_ENABLE
	/*!
	 * \var 		dataIndex
	 * \brief		Index in which the pin data is "currently".
	 */
	static uint8_t dataIndex = 0;
	/*!
	 * \var 		confirmation
	 * \brief		Constant variable which only purpose is to have a space in memory
	 * 				to refer for the UART_bfnSend() function
	 */
	static uint8_t confirmation = 1;
#endif

/*!
 	 \var		pinData
 	 \brief		Stored data introduced with the bluetooth
*/
static uint8_t pinData[4] = {0};

/*!
 * \var 		password
 * \brief		Stores the initial password
 */
static uint8_t password[4] = {1, 2, 3, 4};
//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------
/*!
 * \fn 			void Password_vfnDriverInit ()
 * \brief		Initialize the necesary ports and drivers for the main program
 */
void Password_vfnDriverInit ()
{
  	/* Init board hardware. */
	// Initialize required ports for the matrix to work
	Matrix_vfnPortInit();

	// Initialize required ports and optional functions for the bluetooth module
#ifdef BLUETOOTH_INTERRUPT_ENABLE
	UART_vfnCallbackReg(readVal);
#endif

	UART_vfnDriverInit();

}
/*!
 * \fn			uint8_t Password_bfnIsCorrect(void)
 * \return		Returns a 1 if the introduced password is correct; else, returns 0
 * \brief		This function, when called, evaluates if the introduced password,
 * 				either from the keyboard or from the bluetooth module.
 */
uint8_t Password_bfnIsCorrect(void)
{
	// To-Do
	uint8_t i = 0;
	uint8_t isCorrect = 1;
	for (i = 0; i < 4; i++)
	{
		if (!(pinData[i] == password[i]))
		{
			isCorrect = 0;
		}
	}

	return isCorrect;
}

//------------------------------------------------------------------------------
// Matrix functions
//------------------------------------------------------------------------------
/*!
    \fn			uint8_t Matrix_bfnGetChar(void)
    \return		Returns a char type character
    \brief  	This function returns the character pressed in the matrix keypad
*/
uint8_t Matrix_bfnGetChar(void)
{
	// Define variables
	uint8_t row = 0;
	uint8_t column = 0;

	if (Matrix_bfnMatrixRead(&row, &column))
	{
		return matrix[row][column];
	}
	else
	{
		return 0;
	}
}

/*!
    \fn			void Matrix_vfnPortInit (void)
    \brief		Initialize the ports used to connect the matrix keypad in the KL27z
*/
void Matrix_vfnPortInit (void)
{
	//////////////////////// Initialize ports as GPIOs ////////////////////////
	// Rows 0 - 3 (Outputs)
	GPIO_vfnPortInit(ePORTD, ePIN0, eOUTPUT);
	GPIO_vfnPortInit(ePORTD, ePIN1, eOUTPUT);
	GPIO_vfnPortInit(ePORTD, ePIN2, eOUTPUT);
	GPIO_vfnPortInit(ePORTD, ePIN3, eOUTPUT);

	// Columns 0 - 2 (Inputs)
	GPIO_vfnPortInit(ePORTD, ePIN4, eINPUT);
	GPIO_vfnPortInit(ePORTD, ePIN5, eINPUT);
	GPIO_vfnPortInit(ePORTB, ePIN3, eINPUT);
}

/*!
    \fn			uint8_t Matrix_bfnMatrixRead (uint8_t *row, uint8_t *column)
    \param		row		Pointer to a variable where the value of the active pressed row will be stored
    \param		column	Pointer to a variable where the value of the active pressed row will be stored
    \return		If a button is pressed, returns 1; else, returns 0
    \brief		Iterates through all rows and columns to find the pressed button in the keypad
*/
uint8_t Matrix_bfnMatrixRead (uint8_t *row, uint8_t *column)
{
	*row = 0;
	*column = 0;
	for (*row = 0; *row < ROWS; (*row)++)
	{
		Matrix_bfnPorts(eOUTPUT, *row, ON);
		for (*column = 0; *column < COLUMNS; (*column)++)
		{
			if (Matrix_bfnPorts(eINPUT, *column, OFF))
			{
				Matrix_bfnPorts(eOUTPUT, *row, OFF);
				return 1;
			}
//			printf("%d\t", matrix[*row][*column]);
		}
		Matrix_bfnPorts(eOUTPUT, *row, OFF);
//		printf("\n");
	}
//	printf("\n");
	return 0;
}

/*!
    \fn			uint8_t Matrix_bfnPorts(IO io, uint8_t iteration, uint8_t onOff)
    \param		io			Parameter to set if the pin you're using is as INPUT or OUTPUT
    \param		iteration	Parameter for receiving the number of the iteration currently going in the Matrix_bfnMatrixRead loop
    \param		onOff		Parameter for receiving if the port you will use as OUTPUT, is going to be ON or OFF
    \return		If io is 1, always returns 1; if io is 0, returns the value read from the selected pin, either 1 or 0
    \brief		For internal use only for the Matrix_bfnMatrixRead function
*/
uint8_t Matrix_bfnPorts(IO io, uint8_t iteration, uint8_t onOff)
{
	uint8_t result = 0;

	if (io)
	{
		switch (iteration)
		{
		case 0:
			GPIO_bfnData(ePORTD, ePIN0, &onOff);
			break;

		case 1:
			GPIO_bfnData(ePORTD, ePIN1, &onOff);
			break;

		case 2:
			GPIO_bfnData(ePORTD, ePIN2, &onOff);
			break;

		case 3:
			GPIO_bfnData(ePORTD, ePIN3, &onOff);
			break;

		default:
			break;
		}
		return 1;
	}
	else
	{
		switch (iteration)
		{
		case 0:
			GPIO_bfnReadData(ePORTD, ePIN4, &result);
			break;

		case 1:
			GPIO_bfnReadData(ePORTD, ePIN5, &result);
			break;

		case 2:
			GPIO_bfnReadData(ePORTB, ePIN3, &result);
			break;

		default:
			break;
		}

		return result;
	}
}

//------------------------------------------------------------------------------
// Bluetooth functions
//------------------------------------------------------------------------------
/*!
 	 \fn		void readVal()
 	 \brief		Function to be executed when an interruption is generated by the UART driver
 */
#ifdef BLUETOOTH_INTERRUPT_ENABLE
void readVal()
{
	if(UART_bfnRead(&pinData[dataIndex]))
	{
		printf("%d\n", pinData[dataIndex]);
		UART_bfnSend(&confirmation);
		if (3 <= dataIndex)
		{
			dataIndex = 0;
		}
		else
		{
			dataIndex++;
		}
	}
}
#endif
//------------------------------------------------------------------------------
