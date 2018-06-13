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
/// \file 	ISZ2510.h
/// \author	Jean-Sebastien Gonsette
/// \date	17/10/2014
///
// ###########################################################################

#ifndef _ISZ2510_H
#define _ISZ2510_H


// ===========================================================================
// I N C L U D E
// ===========================================================================

#include "SDK.h"



// ###########################################################################
//
// T Y P E S
//
// ###########################################################################

/// Function to select a SPI device (electronically) 
typedef void (*ISZ_SPI_CS) (int level);


// ===========================================================================
/// ISZ2510 lowpass filter bandwidth
// ===========================================================================
typedef enum
{
	ISZ2510_BW_5 = 6,		///< 5 Hz LPF
	ISZ2510_BW_10 = 5,		///< 10 Hz LPF
	ISZ2510_BW_20 = 4,		///< 20 Hz LPF
	ISZ2510_BW_41 = 3,		///< 41 Hz LPF
	ISZ2510_BW_92 = 2,		///< 92 Hz LPF
	ISZ2510_BW_184 = 1,		///< 184 Hz LPF
	ISZ2510_BW_250 = 0,		///< 250 Hz LPF
	ISZ2510_BW_3600 = 7,	///< 3600 Hz LPF
	ISZ2510_BW_8800 = 8		///< 8880 Hz LPF	
}
ISZ2510_Bandwidth;


// ===========================================================================
/// ISZ2510 Activity mode
// ===========================================================================
typedef enum
{
	ISZ2510_MODE_SLEEP,		///< Sleep mode
	ISZ2510_MODE_STANDBY,	///< No acquisition
	ISZ2510_MODE_MEASURE	///< Acquire data
}
ISZ2510_Mode;


// ===========================================================================
/// ISZ2510 Gyro scale
// ===========================================================================
typedef enum
{
	ISZ2510_SCALE_250 = 0,	///< +- 250 °/s
	ISZ2510_SCALE_500 = 1,	///< +- 500 °/s
	ISZ2510_SCALE_1000 = 2,	///< +- 1000 °/s
	ISZ2510_SCALE_2000 = 3	///< +- 2000 °/s
}
ISZ2510_Scale;


// ===========================================================================
/// ISZ calibration values
// ===========================================================================
typedef struct
{
	Word offset;
	Word scaler;
}
ISZ2510_CalibValues;


// ===========================================================================
/// ISZ2510 device configuration structure
// ===========================================================================
typedef struct
{
	HANDLE spi;					///< SPI bus handle
	ISZ_SPI_CS fCS;				///< CS pin selection function
}
ISZ2510_Config;


// ===========================================================================
// ISZ2510 device control bloc
// ===========================================================================
typedef struct
{
	HANDLE spi;				///< SPI bus handle
	ISZ_SPI_CS fCS;			///< CS pin selection function
	
	ISZ2510_Scale scale;	///< Selected scale
	Word scaler;			///< Scale factor correction for each axis (x2^15)
}	
ISZ2510_DCB;



// ###########################################################################
//
// P R O T O T Y P E S
//
// ###########################################################################

HANDLE ISZ2510_OpenDriver ();
HANDLE ISZ2510_CreateDevice (const char* name, ISZ2510_Config* pConfig);
void ISZ2510_CloseDevice (HANDLE device);

int ISZ2510_Init (HANDLE device);
void ISZ2510_Reset (HANDLE device);
Bool ISZ2510_Selftest (HANDLE device);
void ISZ2510_SetMode (HANDLE device, ISZ2510_Mode mode);
void ISZ2510_SetScale (HANDLE device, ISZ2510_Scale);
Short ISZ2510_GetTemperature (HANDLE device);
void ISZ2510_SetBandwidth (HANDLE device, ISZ2510_Bandwidth bw);
Word ISZ2510_SetBaudrate (HANDLE device, Word br);
Long ISZ2510_GetAngularSpeed (HANDLE device);
void ISZ2510_SetCalibrationOffset (HANDLE device, Long offset);
void ISZ2510_GetCalibrationValues (HANDLE device, ISZ2510_CalibValues* calib);
void ISZ2510_SetCalibrationValues (HANDLE device, ISZ2510_CalibValues calib);

#endif
