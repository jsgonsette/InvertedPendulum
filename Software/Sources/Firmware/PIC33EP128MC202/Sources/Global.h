// ###########################################################################
//
// Copyright (C) 2015 GenId SPRL
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///
/// \file 	Global.h
/// \author	Jean-Sebastien Gonsette
/// \date	17/09/2014
///
// ###########################################################################

#ifndef _GLOBAL_H
#define _GLOBAL_H


// ===========================================================================
// I N C L U D E
// ===========================================================================

#include "NanoScale.h"



// ###########################################################################
//
// G L O B A L   C O N F I G U R A T I O N
//
// ###########################################################################

/// Do we use DEBUG mode ?
#define DEBUG_MODE

/// Minimum battery voltage to run [1/100 V]
#define BAT_MIN_VOLTAGE		700

/// Battery capacity [mA.h]
#define BAT_CAPACITY		450

/// Base period of the LED state machine [ms]
#define LED_SM_PERIOD		100

/// UART port baudrate
#define UART_BAUDRATE		115200



// ###########################################################################
//
// A P P L I C A T I O N   E R R O R S
//
// ###########################################################################

#define ERR_APPLICATION         1000
#define ERR_ASSERTION           ERR_APPLICATION +0
#define ERR_ADXL345             ERR_APPLICATION +1
#define ERR_ISZ2510             ERR_APPLICATION +2
#define ERR_UART                ERR_APPLICATION +3
#define ERR_SPI                 ERR_APPLICATION +4
#define ERR_UART_QUEUE			ERR_APPLICATION +5



// ###########################################################################
//
// G L O B A L
//
// ###########################################################################

extern HANDLE g_taskUartTx;			///< Task for Uart Tx management
extern HANDLE g_taskUartRx;			///< Task for Uart Rx management
extern HANDLE g_taskLED;			///< Task for LED management
extern HANDLE g_taskMain;			///< Main application task

extern HANDLE g_UART;				///< UART1 peripheral
extern HANDLE g_SPI;				///< SPI1 peripheral
extern HANDLE g_ADXL345;			///< XYZ Accelerometer peripheral
extern HANDLE g_ISZ2510;			///< Z Gyro peripheral


#endif

// End
