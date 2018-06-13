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
/// \file 	Bsp.h
/// \author	Jean-Sebastien Gonsette
/// \date	17/09/2014
///
// ###########################################################################

#ifndef _BSP_H
#define _BSP_H


// ===========================================================================
// I N C L U D E
// ===========================================================================
#include <xc.h>
#include "NanoScale.h"



// ###########################################################################
//
// T Y P E S
//
// ###########################################################################



// ###########################################################################
//
// P R O T O T Y P E S
//
// ###########################################################################

int BSP_Init ();
int BSP_InitDevices ();

void BSP_BLDCSetEnable (int enable);
void BSP_BLDCSetBrake (int brake);
void BSP_BLDCSetForward (int fwd);
void BSP_BLDCSetForwardDuty (Short duty);
void BSP_BLDCSetRef (Word ref);
Word BSP_BLDCGetSpeed (Word *counter);

void BSP_LEDSetRed (int s);
void BSP_LEDSetGreen (int s);
void BSP_LEDSetYellow (int s);
Word BSP_BatGetVoltage ();

Dword BSP_GetClockRate ();
Bool BSP_WriteEeprom (const Byte* data, int len);
void BSP_ReadEeprom (Byte* data, int len);

#endif
// End
