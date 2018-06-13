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
/// \file 	CtrlJump.h
/// \author	Jean-Sebastien Gonsette
/// \date	20/12/2014
///
// ###########################################################################

#ifndef _CTRLJUMP_H
#define _CTRLJUMP_H


// ===========================================================================
// I N C L U D E
// ===========================================================================

#include "NanoScale.h"
#include "./Tasks/Controller.h"



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

void CTRLJUMP_Restart (Word dt);
CTRL_Command CTRLJUMP_ComputeCommand (CTRL_State);


#endif
