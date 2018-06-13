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
/// \file 	TaskMain.h
/// \author	Jean-Sebastien Gonsette
/// \date	20/10/2014
///
// ###########################################################################

#ifndef _TASKMAIN_H
#define _TASKMAIN_H


// ===========================================================================
// I N C L U D E
// ===========================================================================

#include "NanoScale.h"



// ###########################################################################
//
// T Y P E S
//
// ###########################################################################

// ===========================================================================
/// Application mode
// ===========================================================================
typedef enum
{
	MODE_Stop = 0,			///< Controller is stopped
	MODE_Free,				///< No control, simple estimation of pendulum angle
	MODE_Control			///< Controlling pendulum angle
}
TMAIN_Mode;



// ###########################################################################
//
// P R O T O T Y P E S
//
// ###########################################################################

void TMAIN_EntryPoint (void* param);

void TMAIN_SetTelemetry (int n);
void TMAIN_SetMode (TMAIN_Mode mode);

void TMAIN_CalibrateBattery (Word refVoltage);
void TMAIN_CalibrateIMU (Long accScales [2], Long accBias [3], Long gyroBias);

#endif
