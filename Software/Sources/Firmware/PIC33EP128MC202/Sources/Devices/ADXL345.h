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
/// \file 	ADXL345.h
/// \author	Jean-Sebastien Gonsette
/// \date	11/10/2014
///
// ###########################################################################

#ifndef _ADXL345_H
#define _ADXL345_H


// ===========================================================================
// I N C L U D E
// ===========================================================================

#include "SDK.h"



// ###########################################################################
//
// T Y P E S
//
// ###########################################################################

/// Fonction permettant de sélectionner un périphérique sur le bus SPI
typedef void (*SPI_CS) (int level);


// ===========================================================================
/// ADXL345 Baudrate option
// ===========================================================================
typedef enum
{
	ADXL345_BR_0_10 = 0,	///< 0.10 Hz
	ADXL345_BR_0_20,		///< 0.20 Hz
	ADXL345_BR_0_39,		///< 0.39 Hz
	ADXL345_BR_0_78,		///< 0.78 Hz
	ADXL345_BR_1_56,		///< 1.56 Hz
	ADXL345_BR_3_13,		///< 3.13 Hz
	ADXL345_BR_6_25,		///< 6.25 Hz
	ADXL345_BR_12_5,		///< 12.5 Hz
	ADXL345_BR_25,			///< 25 Hz
	ADXL345_BR_50,			///< 50 Hz
	ADXL345_BR_100,			///< 100 Hz
	ADXL345_BR_200,			///< 200 Hz
	ADXL345_BR_400,			///< 400 Hz
	ADXL345_BR_800,			///< 800 Hz
	ADXL345_BR_1600,		///< 1600 Hz
	ADXL345_BR_3200,		///< 3200 Hz
	ADXL345_BR_Unknown
	
}
ADXL345_Baudrate;


// ===========================================================================
/// ADXL345 Activity mode
// ===========================================================================
typedef enum
{
	ADXL345_MODE_SLEEP,		///< Sleep mode, low sampling for activity detection
	ADXL345_MODE_IDLE,		///< No acquisition
	ADXL345_MODE_MEASURE	///< Acquire data
}
ADXL345_Mode;


// ===========================================================================
/// ADXL calibration values
// ===========================================================================
typedef struct
{
	Word offsets [3];
	Word scalers [3];
}
ADXL345_CalibValues;


// ===========================================================================
/// ADXL345 device configuration structure
// ===========================================================================
typedef struct
{
	HANDLE spi;					///< SPI bus handle
	SPI_CS fCS;					///< CS pin selection function
}
ADXL345_Config;


// ===========================================================================
/// ADXL345 Device Control Block
// ===========================================================================
typedef struct
{
	HANDLE spi;				///< SPI bus handle
	SPI_CS fCS;				///< CS pin selection function
	
	Word scalers [3];		///< Scale factor correction for each axis (x2^15)
}
ADXL345_DCB;



// ###########################################################################
//
// P R O T O T Y P E S
//
// ###########################################################################

HANDLE ADXL345_OpenDriver ();
HANDLE ADXL345_CreateDevice (const char* name, ADXL345_Config* pConfig);
void ADXL345_CloseDevice (HANDLE device);

int ADXL345_Init (HANDLE device);

void ADXL345_SetMode (HANDLE device, ADXL345_Mode mode);
void ADXL345_SetLowPowerMode (HANDLE device, int state);
void ADXL345_SetBaudrate (HANDLE device, ADXL345_Baudrate baudrate);
void ADXL345_GetAccelerations (HANDLE device, Short* accX, Short* accY, Short* accZ);

void ADXL345_SetCalibrationOffset (HANDLE device, int axis, Short offset);
void ADXL345_SetCalibrationScaler (HANDLE device, int axis, float scaler);

void ADXL345_GetCalibrationValues (HANDLE device, ADXL345_CalibValues* calib);
void ADXL345_SetCalibrationValues (HANDLE device, ADXL345_CalibValues calib);

#endif
