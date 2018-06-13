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
/// \file	Battery.c
/// \author	Jean-Sebastien Gonsette
/// \date	20/10/2014
///
/// This module provides access to the battery management.
// ###########################################################################

#include "BSP.h"
#include "Global.h"
#include "Battery.h"


// ===========================================================================
// D E F I N E
// ===========================================================================


// ===========================================================================
// T Y P E S
// ===========================================================================



// ===========================================================================
// P R I V A T E
// ===========================================================================

/// Voltage measurement calibration value (x2^15)
static Word gs_calib = 0x8000;

/// Battery capacity [mAh]
static Word gs_capacity;


// ===========================================================================
// P R I V A T E   P R O T O
// ===========================================================================

/// \name Internal functions
//@{
//@}



// ###########################################################################
//
// P U B L I C
//
// ###########################################################################

// ===========================================================================
// <BAT_Init>
///
/// \brief		Set the battery characteristics
///
/// \param	capacity	Battery capacity in [mAh]
// ===========================================================================
void BAT_Init (Word capacity)
{
	gs_capacity = capacity;
}


// ===========================================================================
// <BAT_GetVoltage>
///
/// \brief		Returns battery voltage
///
/// \return		Battery voltage [1/100 V]
// ===========================================================================
Word BAT_GetVoltage ()
{
	Dword voltage;
	
	// Read battery voltage and convert it in 1/100V
	voltage = BSP_BatGetVoltage ();
	voltage *= 1000;
	voltage >>= 12;
	
	// Apply calibration factor
	voltage *= gs_calib;
	voltage >>= 15;	
	
	return (Word) voltage;
}


// ===========================================================================
// <BAT_Calibrate>
///
/// \brief		Calibrate the battery voltage measurement
///
/// \param[in]	Reference voltage measurement [1/100 V]
// ===========================================================================
void BAT_Calibrate (Word refVoltage)
{
	int i;
	Word raw = 0;
	Dword temp;
	
	// Acquire current battery voltage
	for (i = 0; i < 8; i ++) raw += BSP_BatGetVoltage ();
	raw >>= 3;
	
	// Nominal scaling
	temp = ((Dword) raw * 1000) >> 12;
	
	// Compute scaler (0.5 < scaler < 2)
	temp = (((Dword) refVoltage) << 15) / temp;
	if (temp >= 0x10000 || temp <= 0x4000) gs_calib = 0x8000;
	else gs_calib = (Word) temp;
}


// ===========================================================================
// <BAT_GetCalibrationValue>
///
/// \brief		Return the battery calibration value for backup purpose
///
/// \return		Calibration value
// ===========================================================================
Word BAT_GetCalibrationValue ()
{
	return gs_calib;
}


// ===========================================================================
// <BAT_SetCalibrationValue>
///
/// \brief		Restore the battery calibration value
///
/// \param[in]	Calibration value
// ===========================================================================
void BAT_SetCalibrationValue (Word calibValue)
{
	gs_calib = calibValue;
}


// ===========================================================================
// <BAT_GetLipoSOC>
///
/// \brief		Estimate the state of charge of a LIPO battery
///
/// \param[in]	voltage		Battery voltage (now) [1/100 V]
/// \param[in]	current		Battery delivered current (now) [mA]
///
/// \param[in]	Battery SOC [%]
// ===========================================================================
Word BAT_GetLipoSOC (Word voltage, Word current)
{
	Dword v;
	Word t;
	Word n;
	Word vn;
	
	// Correct voltage according to the delivered current
	// 1% voltage drop by delivered current equal to capacity
	v = (Dword) voltage * 100 * gs_capacity;
	v /= ((Dword) gs_capacity*100 - current);
	
	// Compute number of cell elements
	// Then compute nominal voltage (1 cell is 3.7V)
	n = 2*voltage/370;
	if (n & 0x01) n = (n>>1) +1;
	else n >>= 1;
	vn = n*370;
	
	// 100% SOC is equal to 1.145 x nominal voltage
	// 20% SOC is equal to 1 x nominal voltage
	// 0% SOC is equal to 0.8 x nominal voltage
	if (v >= vn)
	{
		t = n*424;
		if (v >= t) return 100;
		return 20 + ((v-vn)*80)/(t-vn);
	}
	else
	{
		t = n*296;
		if (v <= t) return 0;
		return ((v-t)*20) / (vn-t);
	}	
}



// ###########################################################################
//
// P R I V A T E
//
// ###########################################################################

// End
