// ###########################################################################
//
// N A N O S C A L E   R T O S
//
/// \file		NanoScaleDrivers_PIC16.h
/// \version	2.3.1
/// \date		11/04/2013
/// \author		GenId SPRL
///
/// This file contains the API definition of NanoScale Drivers for
/// PIC24 and dsPIC Microchip families
//
//  ###########################################################################

#ifndef _NANOSCALEDRIVERSP16_H
#define _NANOSCALEDRIVERSP16_H

#include "NanoScaleDrivers.h"



// ###########################################################################
//
// D E F I N E S
//
// ###########################################################################

// ===========================================================================
// U A R T   C o n f i g u r a t i o n   F l a g s
// ===========================================================================
#define		UART_MODE_8BITS_ODD_PARITY		2
#define		UART_MODE_8BITS_EVEN_PARITY		1
#define		UART_MODE_8BITS_NO_PARITY		0

#define 	UART_STOPBITS_2					1
#define 	UART_STOPBITS_1					0

#define		UART_RTS_READY_TO_SEND			1
#define		UART_RTS_READY_TO_RECEIVE		0

#define		UART_CFLOW_NONE					0
#define		UART_CFLOW_RTS					1
#define		UART_CFLOW_RTS_CTS				2



// ###########################################################################
//
// T Y P E S
//
// ###########################################################################

// ===========================================================================
// I 2 C
// ===========================================================================
/// I2C configuration structure
typedef struct
{
	unsigned int StopInIdle:			1;		///< Stop operation when processor is IDLE
	unsigned int DisableSlewRate:		1;		///< Disable slew rate control
	unsigned int EnableSMBusThreshold:	1;		///< Enable SMBus specification thresholds
	unsigned int EnableGeneralCall:		1;		///< Enable general call address reception (slave)
	unsigned int EnableClockStretching:	1;		///< Enable clock stretching when receiving data (slave)
	unsigned int IsrPriority:			3;		///< ISR priority for I2C management
}
I2CPIC16_Config;
	

// ===========================================================================
// U A R T
// ===========================================================================
/// UART device configuration structure
typedef struct
{
	unsigned int StopInIdle:		1;		///< Stop operation when processor is IDLE
	unsigned int EnableWakeUp:		1;		///< Wake up device from sleep when a character is received
	unsigned int EnableLoopBack:	1;		///< Loop back de Tx sur Rx
	unsigned int InverseRxPolarity:	1;		///< Invertion of Rx bit polarity
	unsigned int InverseTxPolarity:	1;		///< Invertion of Tx bit polarity
	unsigned int EnableTx:			1;		///< Activate Tx
	unsigned int Mode:				2;		///< Number of bits and parity mode
	unsigned int StopBits:			1;		///< Number of stop bits
	unsigned int RtsMode:			1;		///< Behavior of RTS pin
	unsigned int ControlFlow:		2;		///< Usage of CTS/RTS pins
	unsigned int IsrPriority:		3;		///< ISR priority for UART management

	Word txFifoLength;						///< Tx FIFO size (Bytes)
	Word rxFifoLength;						///< Rx FIFO size (Bytes)
}
UartPIC16_Config;
	
	
// ===========================================================================
// S P I
// ===========================================================================
/// SPI device configuration structure
typedef struct
{
	unsigned int StopInIdle:		1;		///< Stop operation when processor is IDLE
	unsigned int DisableSDO:		1;		///< Disable the SDO output (Receive only)
	unsigned int EnableSS:			1;		///< Slave Select pin used by SPI driver
	unsigned int IsrPriority:		3;		///< ISR priority for SPI management
	
}
SpiPIC16_Config;
	
	
	
// ###########################################################################
//
// P R O T O T Y P E S
//
// ###########################################################################

// I 2 C

NS_API HANDLE I2CPIC16_OpenDriver ();

NS_API HANDLE I2CPIC16_CreateDevice (const char* name, int i2cNumber, I2CPIC16_Config* pConfig);

NS_API void I2CPIC16_CloseDevice (HANDLE device);


// U A R T

NS_API HANDLE UartPIC16_OpenDriver ();

NS_API HANDLE UartPIC16_CreateDevice (const char* name, int uartNumber, UartPIC16_Config* pConfig);

NS_API void UartPIC16_CloseDevice (HANDLE device);


// S P I

NS_API HANDLE SpiPIC16_OpenDriver ();

NS_API HANDLE SpiPIC16_CreateDevice (const char* name, int busNumber, SpiPIC16_Config* pConfig);

NS_API void SpiPIC16_CloseDevice (HANDLE device);


#endif

// End
