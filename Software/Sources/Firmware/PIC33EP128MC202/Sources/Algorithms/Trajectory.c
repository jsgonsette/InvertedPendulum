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
/// \file	Trajectory.c
/// \author	Jean-Sebastien Gonsette
/// \date	05/02/2015
///
/// This module computes trajectories that can be followed by the pendulum
// ###########################################################################

#include "Debug.h"
#include "Pendulum.h"
#include "Trajectory.h"
#include "./Tools/FastTrigo.h"


// ===========================================================================
// D E F I N E
// ===========================================================================


// ===========================================================================
// T Y P E S
// ===========================================================================

/// Types of trajectory
typedef enum
{
	TRJ_Swing,
	TRJ_Walk
}
E_TrjType;

/// Swing trajectory parameters
typedef struct
{
	float w;		///< Swing frequency [rad/s]
	float A;		///< Swing amplitude [rad]
}
TrjSwing;


// ===========================================================================
// P R I V A T E
// ===========================================================================

/// Swing trajectory parameters
static TrjSwing gs_trjSwing;

/// Current trajectory type
static E_TrjType gs_trjType;

/// Time elapsed since beginning of a trajectory
static float gs_time;

/// Current estimated wheel speed
static float gs_wheelSpeed;


// ===========================================================================
// P R I V A T E   P R O T O
// ===========================================================================

/// \name Internal functions
//@{
static void GetSwingState (float time, float *bodyAngle, float *bodySpeed, float *bodyAcc);
static void GetWalkState (float time, float *bodyAngle, float *bodySpeed, float *bodyAcc);
static float Follow (float dt, float bodyAngle, float bodySpeed, float bodyAcc, float *pWheelSpeed);
//@}



// ###########################################################################
//
// P U B L I C
//
// ###########################################################################

// ===========================================================================
// <TRJ_InitSwing>
///
/// \brief		Restart the trajectory generator to follow a "periodic swing"
///
/// \param		period		Periodicity of the swing [ms]
/// \param		amplitude	Amplitude of the swing [°]
// ===========================================================================
void TRJ_InitSwing (Word period, Word amplitude)
{
	gs_trjType = TRJ_Swing;
	
	// Attributes of the swing
	gs_trjSwing.w = 2.0f*PI*1e3f / (float)(period);
	gs_trjSwing.A = DEG(amplitude);
	
	gs_time = 0.0f;
	//gs_wheelSpeed = 0.0f;
    // Cannot control around 0
	gs_wheelSpeed = RPM (1000);
}


// ===========================================================================
// <TRJ_NextStep>
///
/// \brief		Compute the next following pendulum target state for the
///				current trajectory.
///
/// \param		dt			Time step [ms]
/// \param[out]	trqCmd		Nominal torque for this time step [µN]
///
/// \return		Nominal pendulum state for this time step.
// ===========================================================================
CTRL_State TRJ_NextStep (Word dt, Long *trqCmd)
{
	CTRL_State state;
	float angle, speed, acc;
	float torque, dtf;
 
	dtf = dt/1000.0f;
	gs_time += dtf;

	// Next body state for the trajectory
	switch (gs_trjType)
	{
		default: ASSERT (0);
		
		case TRJ_Swing: GetSwingState (gs_time, &angle, &speed, &acc); break;
		case TRJ_Walk: GetWalkState (gs_time, &angle, &speed, &acc); break;
	}

	// Get torque and state resulting from the trajectory
	torque = Follow (dtf, angle, speed, acc, &gs_wheelSpeed);
	
	// Scale result
	state.bodyAngle = (Short) _2_DECI_DEG (angle);
	state.bodySpeed = (Short) _2_DECI_DEG (speed);
	state.wheelSpeed = (Short) _2_RPM (acc);
	*trqCmd = (Long)(1e6f*torque);
	
	return state;
}



// ###########################################################################
//
// P R I V A T E
//
// ###########################################################################

// ===========================================================================
// <GetWalkState>
///
/// \brief	Compute ideal pendulum angle, angular speed and acceleration for
///			the "Walk" trajectory, at a given time
///
/// \param		time		Current time since the beginning of the trajectory [s]
/// \param[out]	bodyAngle	Pendulum body angle [rad]
/// \param[out]	bodySpeed	Pendulum body angular speed [rad/s]
/// \param[out]	bodyAcc		Pendulum body angular acceleration [rad/s^2]
// ===========================================================================
static void GetWalkState (float time, float *bodyAngle, float *bodySpeed, float *bodyAcc)
{
	
}


// ===========================================================================
// <GetSwingState>
///
/// \brief	Compute ideal pendulum angle, angular speed and acceleration for
///			the "Swing" trajectory, at a given time
///
/// \param		time		Current time since the beginning of the trajectory [s]
/// \param[out]	bodyAngle	Pendulum body angle [rad]
/// \param[out]	bodySpeed	Pendulum body angular speed [rad/s]
/// \param[out]	bodyAcc		Pendulum body angular acceleration [rad/s^2]
// ===========================================================================
static void GetSwingState (float time, float *bodyAngle, float *bodySpeed, float *bodyAcc)
{
	float amplitude;
	const float maxTime = 5.0f;
	
	// Reach max amplitude progressively
	amplitude = gs_trjSwing.A;
	if (time < maxTime) amplitude= gs_trjSwing.A * time / maxTime;
	
	// Follow a sinusoidal trajectory
	*bodyAngle = amplitude * FT_Sin (gs_trjSwing.w*time);
	*bodySpeed = amplitude * gs_trjSwing.w * FT_Cos (gs_trjSwing.w*time);
	*bodyAcc = - gs_trjSwing.w * gs_trjSwing.w * (*bodyAngle);
}


// ===========================================================================
// <Follow>
///
/// \brief		Compute the required torque needed to follow a trajectory.
///
/// Trajectory is given by the current state of the pendulum body (angle, speed, acc).
/// The function just assumes that these values are correct and computes
/// the torque needed to compensate for gravity torque, speed friction and still
/// achieve required acceleration.
///
/// \param[in]		dt			Time step [s]
/// \param[in]		bodyAngle	Body angle at this time step [rad]]
/// \param[in]		bodySpeed	Body angular speed at this time step [rad/s]
/// \param[in]		bodyAcc		Body angular acceleration at this time step [rad/s^2]
/// \param[in,out]	pWheelSpeed	Pendulum wheel speed at and after this time step[rad/s]
///
/// \return		Required torque to follow the trajectory [N]
// ===========================================================================
static float Follow (float dt, float bodyAngle, float bodySpeed, float bodyAcc, float *pWheelSpeed)
{
	float torque, wheelAcc;
	const PDL_Physics* p;
 
	p = PDL_GetPhysics ();	
	
	// Compute torque to follow this trajectory
	torque = p->M*G*FT_Sin(bodyAngle) - p->body.Cb*bodySpeed + p->motor.Cw*(*pWheelSpeed) - bodyAcc*p->I;
	
	// Resulting wheel acceleration
	wheelAcc = (p->I+p->motor.I)*(torque - p->motor.Cw*(*pWheelSpeed))/(p->I*p->motor.I) - (p->M*G*FT_Sin(bodyAngle) - p->body.Cb*bodySpeed)/p->I;
	(*pWheelSpeed) += wheelAcc*dt;
	
	return torque;	
}

// End
