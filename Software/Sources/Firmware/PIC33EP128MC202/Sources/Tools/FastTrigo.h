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
/// \file 	FastTrigo.h
/// \author	Jean-Sebastien Gonsette
/// \date	12/10/2014
///
// ###########################################################################

#ifndef _FAST_TRIGO_H
#define _FAST_TRIGO_H

// ===========================================================================
// I N C L U D E
// ===========================================================================

#include "NanoScale.h"


// ===========================================================================
// D E F I N E
// ===========================================================================



// ###########################################################################
//
// P R O T O T Y P E S
//
// ###########################################################################

float FT_Sin (float x);
float FT_Cos (float x);
float FT_Atan2 (float y, float x);
float FT_Exp (float x);
float FT_Sqrt (float x);

Bool IsNaN (const float* f);

#endif

// End
