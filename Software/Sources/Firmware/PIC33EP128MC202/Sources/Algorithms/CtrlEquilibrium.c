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
/// \file	CtrlEquilibrium.c
/// \author	Jean-Sebastien Gonsette
/// \date	01/01/2015
///
/// This module enable to compute the control law in order to hold the
/// pendulum in equilibrium or to make it follow a trajectory.
// ###########################################################################

#include "NanoScale.h"
#include "Pendulum.h"
#include "CtrlEquilibrium.h"
#include "./Tools/FastTrigo.h"


// ===========================================================================
// D E F I N E
// ===========================================================================

#define K1		-7.5107f		///< Feedback gain (angle)
#define K2		-0.5492f		///< Feedback gain (body speed)
#define K3		-0.0018f		///< Feedback gain (wheel speed)

/// Mean wheel speed [RPM]
#define WHEEL_MEAN_SPEED_RPM	(1000.0f)

/// Max. wheel speed [RPM]
#define WHEEL_MAX_SPEED_RPM		(3500)


// ===========================================================================
// T Y P E S
// ===========================================================================


// ===========================================================================
// P R I V A T E
// ===========================================================================

static float delta_zero = 0.0f;


// ===========================================================================
// P R I V A T E   P R O T O
// ===========================================================================

/// \name Internal functions
//@{
static Bool IsValidState (CTRL_State currentState);
//@}



// ###########################################################################
//
// P U B L I C
//
// ###########################################################################

// ===========================================================================
// <CTRLEQUI_Restart>
///
/// \brief		Restart the "jump" controller
///
/// \param		dt			Time step [ms]
// ===========================================================================
void CTRLEQUI_Restart (Word dt)
{
	delta_zero = 0.0f;
}


// ===========================================================================
// <CTRLEQUI_ComputeSteadyCommand>
///
/// \brief		Compute the torque to drive the motor for this time step in 
///				order to maintain the pendulum in a steady state.
///
/// \param		currentState	Current estimated pendulum state
///
/// \return		Controller command
// ===========================================================================
CTRL_Command CTRLEQUI_ComputeSteadyCommand (CTRL_State currentState)
{
	CTRL_Command cmd;
	float body_a, body_w, wheel_w;
	float torque;
	const PDL_Physics* p;
 
	p = PDL_GetPhysics ();
	
	// Safe zero command
	cmd.torque = 0;
	cmd.brake = False;
	cmd.stop = False;
	cmd.end = False;

	// Don't do anything outside our working window
	if (IsValidState (currentState) == False)
	{
		cmd.end = True;
		return cmd;
	}	
	
	// Convert input in mks
	body_a = DECI_DEG (currentState.bodyAngle);
	body_w = DECI_DEG (currentState.bodySpeed);
	wheel_w = RPM (currentState.wheelSpeed);
	
	// Compute state feedback current
	torque = -K1*(body_a - delta_zero) -K2*body_w -K3*wheel_w;
	
	// Update zero angle error
	delta_zero = delta_zero*0.998f + body_a*0.002f;
	
	// Compute torque
	torque *= p->motor.Km;
	
	// Adjust mean wheel speed reference
//	torque += (RPM (WHEEL_MEAN_SPEED_RPM))*(p->motor.Cw + p->motor.Km * K3);

	cmd.torque = (Long)(torque*1e6f);
	return cmd;
}


// ===========================================================================
// <CTRLEQUI_ComputeCommand>
///
/// \brief		Compute the torque to drive the motor for this time step in 
///				order to follow a trajectory.
///
/// \param		currentState	Current estimated pendulum state
/// \param		targetState		Target state for this time step
/// \param		targetTorque	Nominal torque for this time step [µN]
///
/// \return		Controller command
// ===========================================================================
CTRL_Command CTRLEQUI_ComputeCommand (CTRL_State currentState, CTRL_State targetState, Long targetTorque)
{
	CTRL_Command cmd;
	float body_a, body_w, wheel_w;
	float err_1, err_2, err_3;
	float k1, k2, k3;
	float torque;
	const PDL_Physics* p;
 
	p = PDL_GetPhysics ();
	
	// Safe zero command
	cmd.torque = 0;
	cmd.brake = False;
	cmd.stop = False;
	cmd.end = False;
	
	// Don't do anything outside our working window
	if (IsValidState (currentState) == False)
	{
		cmd.end = True;
		return cmd;
	}
	
	// Convert input in mks
	body_a = DECI_DEG (currentState.bodyAngle);
	body_w = DECI_DEG (currentState.bodySpeed);
	wheel_w = RPM (currentState.wheelSpeed);
	
	// Compute error between current state and target
	err_1 = body_a - DECI_DEG (targetState.bodyAngle);
	err_2 = body_w - DECI_DEG (targetState.bodySpeed);
	err_3 = wheel_w - RPM (targetState.wheelSpeed);
	
	// Get gains for the current pendulum state
	k1 = K1;
	k2 = K2;
	k3 = K3;
	
	// Compute state feedback current
	torque = -k1*err_1 -k2*err_2 -k3*err_3;
	
	// Compute torque
	torque *= p->motor.Km;
	
	// Add to nominal torque
	cmd.torque = (Long)(torque*1e6f) + targetTorque;
	
	return cmd;
}



// ###########################################################################
//
// P R I V A T E
//
// ###########################################################################

// ===========================================================================
// <IsValidState>
///
/// \brief	Check whether the current pendulum state is acceptable 
///
/// \param	currentState	Current pendulum state
///
/// \return	True if it is acceptable.
// ===========================================================================
static Bool IsValidState (CTRL_State currentState)
{
	if (currentState.bodyAngle < -300) return False;
	if (currentState.bodyAngle > 300) return False;
	if (currentState.wheelSpeed > WHEEL_MAX_SPEED_RPM) return False;
	if (currentState.wheelSpeed < -WHEEL_MAX_SPEED_RPM) return False;
	return True;
}


// End
