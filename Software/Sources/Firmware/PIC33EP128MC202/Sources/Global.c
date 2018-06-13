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
//
/// \file	Global.c
/// \author	Jean-Sebastien Gonsette
/// \date	17/09/2014
///
/// All the global variables of the project are gathered in this module.
// ###########################################################################

#include "Global.h"


// ===========================================================================
// G L O B A L
// ===========================================================================

// All the global variables stand here
HANDLE g_taskUartTx;		///< Task for Uart Tx management
HANDLE g_taskUartRx;		///< Task for Uart Rx management
HANDLE g_taskLED;			///< Task for LED management
HANDLE g_taskMain;			///< Main application task

HANDLE g_UART;				///< UART1 peripheral
HANDLE g_SPI;				///< SPI1 peripheral
HANDLE g_ADXL345;			///< XYZ Accelerometer peripheral
HANDLE g_ISZ2510;			///< Z Gyro peripheral


// End
