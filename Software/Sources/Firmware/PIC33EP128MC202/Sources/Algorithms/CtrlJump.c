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
/// \file	CtrlJump.c
/// \author	Jean-Sebastien Gonsette
/// \date	20/12/2014
///
/// This module controls the pendulum in order to make a jump.
// ###########################################################################

#include <Math.h>

#include "Pendulum.h"
#include "CtrlJump.h"
#include "./Interfaces/Motor.h"
#include "./Tools/FastTrigo.h"


// ===========================================================================
// D E F I N E
// ===========================================================================

/// Launch speed @ 45°
#define RPM_45		3500

/// Launch speed @ 50°
#define RPM_50		3800




// ===========================================================================
// T Y P E S
// ===========================================================================


// ===========================================================================
// P R I V A T E
// ===========================================================================

/// Target wheel speed before braking [RPM]
static Short gs_targetSpeed;

/// Command to drive the motor
static CTRL_Command gs_cmd;

/// Control time step
static Word gs_dt;

/// 
static Word gs_delay;



// ===========================================================================
// P R I V A T E   P R O T O
// ===========================================================================

/// \name Internal functions
//@{
static Short GetLaunchSpeed (Short bodyAngle);
//@}



// ###########################################################################
//
// P U B L I C
//
// ###########################################################################

// ===========================================================================
// <CTRLJUMP_Restart>
///
/// \brief		Restart the "jump" controller
///
/// \param		dt			Time step [ms]
// ===========================================================================
void CTRLJUMP_Restart (Word dt)
{
	gs_cmd.end = False;
	gs_cmd.brake = False;
	gs_cmd.stop = False;
	gs_cmd.torque = 0;
	
	gs_targetSpeed = 0;
	gs_dt = dt;
}


// ===========================================================================
// <CTRLJUMP_ComputeCommand>
///
/// \brief		Compute the torque to drive the motor for this time step
///
/// \param		currentState	Current estimated pendulum state
///
/// \return		Controller command
// ===========================================================================
CTRL_Command CTRLJUMP_ComputeCommand (CTRL_State currentState)
{
	int e;
	const PDL_Physics* p; 
	float w, trq;
	p = PDL_GetPhysics ();
	
	
	if (gs_cmd.end == True) return gs_cmd;
	
	// If we accelerate the wheel
	if (gs_cmd.brake == False)
	{
		// The first time, we compute the target wheel speed
		if (gs_targetSpeed == 0) 
		{
			gs_targetSpeed = GetLaunchSpeed (currentState.bodyAngle);
			if (gs_targetSpeed > 0) gs_cmd.torque = 2000;
			else gs_cmd.torque = -2000;
		}
		
		// Check the pendulum has not moved
		/// \todo
		
		// Check if target speed is reached
		e = gs_targetSpeed-currentState.wheelSpeed;
		if (gs_targetSpeed < 0) e = -e;
	
		// Accelerate progressively
		if (e > 0) 
		{
			if (gs_targetSpeed > 0) gs_cmd.torque += 100;
			else gs_cmd.torque -= 100;
		}
		
		// Or start jumping
		else 
		{
			gs_cmd.brake = True;
			gs_delay = 2000;
		}
	}
	
	// We brake the wheel to jump up
	else
	{
		// Stop braking when wheel is at rest
		if (/*currentState.wheelSpeed == 0 ||*/ gs_delay < gs_dt)
		{
			gs_cmd.brake = False;
			gs_cmd.torque = 0;
			gs_cmd.end = True;
		}
		
		// Or brake
		else 
		{
			// Estimate nraking torque, depending on speed
			w = RPM (currentState.wheelSpeed);
			trq = MOTOR_GetBrakingTorque (w);
			
			gs_cmd.brake = True;
			gs_cmd.torque = (Long) (1e6f * trq);

			gs_delay -= gs_dt;
		}
	}

	return gs_cmd;
}



// ###########################################################################
//
// P R I V A T E
//
// ###########################################################################

// ===========================================================================
// <GetLaunchSpeed>
///
/// \brief	Compute the speed required to make the jump, 
///			depending on the body angle
///
/// \param		bodyAngle		Body angle [1/10 °]
///
/// \return		Wheel speed [RPM]
// ===========================================================================
static Short GetLaunchSpeed (Short bodyAngle)
{
	float w;
	Short wi;
	Short absAngle;
	
	const PDL_Physics* p; 
	p = PDL_GetPhysics ();

	// Linear interpolation: 3350 RPM @ 45°, 3800 RPM @ 50°
	absAngle = bodyAngle;
	if (bodyAngle < 0) absAngle = -absAngle;
	w = (float) RPM_45 + (absAngle - 450) * (float) (RPM_50-RPM_45) / 50.0f;

	// Convert and limit to max motor speed
	wi = (Short) w;
	if (wi > 4000) wi = 4000;

	// Wheel rotation direction depends on which side the pendulum stays
	if (bodyAngle > 0) wi = -wi;
	
	return wi;
}

// End
