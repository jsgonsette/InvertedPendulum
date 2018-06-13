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
/// \file 	Debug.h
/// \author	Jean-Sebastien Gonsette
/// \date	15/01/2015
///
// ###########################################################################

#ifndef _DEBUG_H
#define _DEBUG_H


// ===========================================================================
// I N C L U D E
// ===========================================================================

#include "NanoScale.h"
#include "Global.h"
#include "./Tasks/Controller.h"



// ###########################################################################
//
// A S S E R T I O N
//
// ###########################################################################

#define ASSERT_UNUSED(x) do { (void)sizeof(x); } while(0)

#ifdef DEBUG_MODE
	#define ASSERT(cond) \
			do \
			{ \
				if (!(cond)) \
				{ \
					ThrowError (ERR_ASSERTION); \
				} \
			} while(0)
#else
	#define ASSERT(cond) \
		do { ASSERT_UNUSED(cond); } while(0)
#endif



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



#endif
