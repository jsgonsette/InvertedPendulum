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
/// \file	Config.c
/// \author	Jean-Sebastien Gonsette
/// \date	21/10/2014
///
/// This module enables to backup and restore application settings in EEPROM
// ###########################################################################

#include "BSP.h"
#include "Global.h"
#include "Config.h"
#include "./Interfaces/Battery.h"
#include "./Interfaces/Motor.h"
#include "./Devices/ADXL345.h"
#include "./Devices/ISZ2510.h"


// ===========================================================================
// D E F I N E
// ===========================================================================


// ===========================================================================
// T Y P E S
// ===========================================================================

/// Configuration structure saved in EEPROM
typedef struct 
{
	Word batCalib;						///< Calibration for the battery voltage
	MOTOR_CalibValues motorCalib;		///< Calibration for the motor device
	ADXL345_CalibValues adxlCalib;		///< Calibration for the ADXL345 device
	ISZ2510_CalibValues iszCalib;		///< Calibration for the ISZ2510 device
	
	Word checksum;						///< Checksum over all the data
}
Config;


// ===========================================================================
// P R I V A T E
// ===========================================================================


// ===========================================================================
// P R I V A T E   P R O T O
// ===========================================================================

/// \name Internal functions
//@{
static Word ComputeChecksum (const Byte* pData, int len);
//@}



// ###########################################################################
//
// P U B L I C
//
// ###########################################################################

// ===========================================================================
// <CFG_Save>
///
/// \brief		Save all the application settings in EEPROM
///
/// \return		0 is successful
// ===========================================================================
int CFG_Save ()
{
	Bool r;
	Config cfg;
	
	EnterCritical ();
	{
		// Get application settings
		cfg.batCalib = BAT_GetCalibrationValue ();
		MOTOR_GetCalibrationValues (&cfg.motorCalib);
		ADXL345_GetCalibrationValues (g_ADXL345, &cfg.adxlCalib);
		ISZ2510_GetCalibrationValues (g_ISZ2510, &cfg.iszCalib);
	}
	LeaveCritical ();

	// Compute a checksum of these data
	cfg.checksum = ComputeChecksum ((Byte*) &cfg, sizeof (cfg) - sizeof (cfg.checksum));
		
	// Put data in EEPROM
	r = BSP_WriteEeprom ((Byte*) &cfg, sizeof (cfg));
	
	if (r) return 0;
	else return -1;
}


// ===========================================================================
// <CFG_Restore>
///
/// \brief		Restore all the application settings from EEPROM
///
/// \return		0 if valid configuration information have been found and loaded
// ===========================================================================
int CFG_Restore ()
{
	Config cfg;
	Word checksum;
	
	// Get data from EEPROM
	BSP_ReadEeprom ((Byte*) &cfg, sizeof (cfg));
		
	// Do nothing if the checksum is wrong
	checksum = ComputeChecksum ((Byte*) &cfg, sizeof (cfg) - sizeof (checksum));
	if (cfg.checksum != checksum) return -1;
	
	// Else, restore application settings
	else
	{
		EnterCritical ();
		{
			BAT_SetCalibrationValue (cfg.batCalib);
			MOTOR_SetCalibrationValues (cfg.motorCalib);
			ADXL345_SetCalibrationValues (g_ADXL345, cfg.adxlCalib);
			ISZ2510_SetCalibrationValues (g_ISZ2510, cfg.iszCalib);	
		}
		LeaveCritical ();
	}

	return 0;
}



// ###########################################################################
//
// P R I V A T E
//
// ###########################################################################

// ===========================================================================
// <ComputeChecksum>
///
/// \brief		Compute a simple checksum over a set of data
///
/// Check sum is just the XOR combination of all the data, Word by Word
///
/// \param	pData	Pointer to the data to process
/// \param	len		Data length
///
/// \return		Checksum
// ===========================================================================
static Word ComputeChecksum (const Byte* pData, int len)
{
	Word *p;
	Word chk = 0x6969;
	
	p = (Word*) pData;
	while (len > 1)
	{
		chk ^= *p;
		p ++;
		len -= 2;
	}
	
	if (len > 0) chk ^= *((Byte*) p);
	return chk;
}


// End
