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
/// \file 	TaskLED.h
/// \author	Jean-Sebastien Gonsette
/// \date	08/10/2014
///
// ###########################################################################

#ifndef _TASKLED_H
#define _TASKLED_H


// ===========================================================================
// I N C L U D E
// ===========================================================================



// ###########################################################################
//
// T Y P E S
//
// ###########################################################################

/// Led identification
typedef enum
{
    LED_RED,
    LED_YELLOW,
    LED_GREEN
}
E_LED;



// ###########################################################################
//
// P R O T O T Y P E S
//
// ###########################################################################

void TLED_EntryPoint (void* param);

void TLED_Off (E_LED led);
void TLED_On (E_LED led);
void TLED_Blink (E_LED led, Byte T);
void TLED_Pulse (E_LED led, Byte numRep);
void TLED_PingPongOut (Byte T);
void TLED_PingPongIn (Byte T);
void TLED_RollLeft (Byte T);
void TLED_RollRight (Byte T);

#endif
