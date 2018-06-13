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
/// \file	CtrlFall.c
/// \author	Jean-Sebastien Gonsette
/// \date	27/03/2015
///
/// This module controls the pendulum in order to make it fall on the ground nicely
// ###########################################################################

#include <Math.h>

#include "Pendulum.h"
#include "CtrlFall.h"
#include "./Tools/FastTrigo.h"


// ===========================================================================
// D E F I N E
// ===========================================================================


// ===========================================================================
// T Y P E S
// ===========================================================================


// ===========================================================================
// P R I V A T E
// ===========================================================================

/// Command to drive the motor
static CTRL_Command gs_cmd;

/// Body angle tracking
static float gs_bodyAngle;

/// Body speed tracking
static float gs_bodySpeed;

/// Running ?
static Bool start;


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
// <CTRLFALL_Restart>
///
/// \brief		Restart the "Fall" controller
// ===========================================================================
void CTRLFALL_Restart ()
{
	gs_cmd.end = False;
	gs_cmd.brake = False;
	gs_cmd.torque = 0;
	gs_cmd.stop = False;
	
	gs_bodyAngle = 0.0f;
	start = False;
}


// ===========================================================================
// <CTRLFALL_ComputeCommand>
///
/// \brief		Compute the torque to drive the motor for this time step
///
/// \param		currentState	Current estimated pendulum state
/// \param		dt				Time step [ms]
///
/// \return		Controller command
// ===========================================================================
CTRL_Command CTRLFALL_ComputeCommand (CTRL_State currentState, Word dt)
{
	float torque;
	float angleRange;
	float mgs, dtf;
	const PDL_Physics* p; 
	
	p = PDL_GetPhysics ();

	// Record start condition
	if (start == False)
	{
		start = True;
		gs_bodyAngle = DECI_DEG (currentState.bodyAngle);
		gs_bodySpeed = DECI_DEG (currentState.bodySpeed);
	}

	angleRange = DEG (45);
	dtf = dt*1e-3f;
	mgs = p->M * G * FT_Sin (gs_bodyAngle);

	// Torque command
	torque = mgs*0.95f;
	gs_cmd.torque = (Long)(torque*1e6f);
	
	// Theoric pendulum angle integration
	gs_bodyAngle += DECI_DEG (currentState.bodySpeed) * dtf; 
	gs_bodySpeed += (mgs - torque) * dtf / p->I;
	
	// Stop condition: angle out of range, wheel speed too high, 
	if (gs_bodyAngle < -angleRange || gs_bodyAngle > angleRange) gs_cmd.end = True;
	if (currentState.wheelSpeed < - 3500 || currentState.wheelSpeed > 3500) gs_cmd.end = True;
	if (gs_bodyAngle < -angleRange/2 || gs_bodyAngle > angleRange/2)
	{
		if (currentState.bodySpeed < 200 && currentState.bodySpeed > -200) gs_cmd.end = True;
	}

	return gs_cmd;
}



// ###########################################################################
//
// P R I V A T E
//
// ###########################################################################


// End
