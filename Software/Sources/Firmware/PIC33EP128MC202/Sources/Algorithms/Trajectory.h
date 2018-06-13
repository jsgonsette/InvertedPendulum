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
/// \file 	Trajectory.h
/// \author	Jean-Sebastien Gonsette
/// \date	05/02/2015
///
// ###########################################################################

#ifndef _TRAJECTORY_H
#define _TRAJECTORY_H


// ===========================================================================
// I N C L U D E
// ===========================================================================

#include "NanoScale.h"
#include "./Tasks/Controller.h"



// ###########################################################################
//
// P R O T O T Y P E S
//
// ###########################################################################

void TRJ_InitSwing (Word period, Word amplitude);
CTRL_State TRJ_NextStep (Word dt, Long *trqCmd);

#endif
