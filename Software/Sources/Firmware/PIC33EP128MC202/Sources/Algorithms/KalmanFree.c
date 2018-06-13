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
/// \file	KalmanFree.c
/// \author	Jean-Sebastien Gonsette
/// \date	03/11/2014
///
/// This module implements a simple Kalman filter. Accelerometer ang gyro 
/// sensors values are fused in order to evaluate the body angle.
// ###########################################################################

#include <math.h>
#include "KalmanFree.h"
#include "Pendulum.h"
#include "./Tools/FastTrigo.h"
#include "./Interfaces/Motor.h"


// ===========================================================================
// D E F I N E
// ===========================================================================

/// Initial body angle uncertainty in [-45;45°]
#define INIT_MAX_ANGLE			(DEG(45))	

/// Initial wheel speed uncertainty in [-1;1rad/s]
#define INIT_MAX_WHEEL_SPEED	(1.0f)		

/// Accelerometer noise variance [(m/s^2)^2]
#define ACC_VAR					(8e-4f)

/// Gyro noise variance [(rad/s)^2]
#define GYRO_VAR				(3e-6f)

/// Modelization error variance [rad^2]
#define MOD_VAR					(3e-8f)


// ===========================================================================
// T Y P E S
// ===========================================================================


// ===========================================================================
// P R I V A T E
// ===========================================================================

/// Body angle
static float gs_theta;

/// Wheel speed
static float gs_w;

/// State error covariance: body angle
static float gs_P;

/// State error covariance: wheel speed
static float gs_Pw;

/// Gyroscope angular speed
static float gs_wz;

/// Last reported motor torque
static float gs_torque;


// ===========================================================================
// P R I V A T E   P R O T O
// ===========================================================================

/// \name Internal functions
//@{
static float PredictWheelSpeedUpdate (float torque, float dt, Bool brake);
//@}



// ###########################################################################
//
// P U B L I C
//
// ###########################################################################

// ===========================================================================
// <KFREE_Init>
///
/// \brief		Init the Kalman filter
// ===========================================================================
void KFREE_Init ()
{
	// Default initial filter values
	gs_theta = 0.0f;
	gs_w = 0.0f;
	gs_wz = 0.0f;
	gs_torque = 0.0f;
	
	// Uncertainty on the initial values
	gs_P = UNIFORM_PDF_VAR (INIT_MAX_ANGLE);
	gs_Pw = UNIFORM_PDF_VAR (INIT_MAX_WHEEL_SPEED);
}


// ===========================================================================
// <KFREE_Update>
///
/// \brief		Apply system update equations to the filter
///
/// \param		gyro		Gyro sensor angular speed [1/1000 °/s]
/// \param		torque		Torque delivered to the motor [uN.m]
/// \param		brake		True if the torque results from braking
/// \param		dt			Time step [ms]
// ===========================================================================
void KFREE_Update (Long gyro, Long torque, Bool brake, Word dt)
{	
	float dtf, w, dw;
	const PDL_Physics* p;
 
	p = PDL_GetPhysics ();
	
	// Convert gyro in rad/s
	gs_wz = MILLI_DEG (gyro);
	dtf = MILLI_SECONDS (dt);
	
	// Body angle state update 
	gs_theta += gs_wz*dtf;
	
	// Keep it between -pi and pi
	if (gs_theta > PI) gs_theta -= PI;
	if (gs_theta < -PI) gs_theta += PI;
	
	// Predict wheel speed update
	gs_torque = torque*1e-6f;
	w = PredictWheelSpeedUpdate (gs_torque, dtf, brake);
	dw = w - gs_w;
	gs_w = w;
		
	// Body angle error covariance update: P = A*P*A^-1	+ Q
	gs_P += dtf*GYRO_VAR + MOD_VAR;

	// Wheel speed error covariance update: P = A*P*A^-1 + Q	
	dw *= 0.1;
	gs_Pw += UNIFORM_PDF_VAR (dw);	
}


// ===========================================================================
// <KFREE_Measure>
///
/// \brief		Apply sensors measurement equations to the filter
///
/// \param		accX		Accelerometer value, X axis [mg]
/// \param		accY		Accelerometer value, Y axis [mg]
// ===========================================================================
void KFREE_Measure (Short accX, Short accY)
{
	float ax, ay;
	float angle;
	float inno;
	float K;
	const PDL_Physics* physics;
 
	physics = PDL_GetPhysics ();
	
	// Convert acceleration to m/s^2
	ax = MILLI_G (accX);
	ay = MILLI_G (accY);
	
	// Remove centripetal force, normally the remaining force is g
	ay += gs_wz*gs_wz*physics->body.ra;
	
	// Angle measurement from gravity force angle in the body frame:
	angle = FT_Atan2 (ax, ay);
	if (angle > PI) angle -= 2.0f*PI;

	// Innovation
	inno = angle - gs_theta;
	
	// Kalman gain
	K = gs_P / (gs_P + ACC_VAR);
	
	// Measurement update
	gs_theta += inno * K;
	gs_P = gs_P - K*gs_P;
}


// ===========================================================================
// <KFREE_MeasureWheel>
///
/// \brief		Apply sensors measurement equations to the filter
///				for BLDC motor speed measurement.
///
/// \param		speed	Motor angular rate [RPM]
///						! This value is unsigned, rotation direction must be
///						guessed !
// ===========================================================================
void KFREE_MeasureWheel (Word speed)
{
	float r, w, K;
	float inno;
	const PDL_Physics* physics;
 
	physics = PDL_GetPhysics ();

	// Speed in [rad/s]
	w = RPM (speed);
	
	// Assume our guess is not too bad so we can deduce direction
	if (gs_w < 0.0f) w = -w;

	// Compute speed error variance
	if (speed == 0) return;
	r = (2.0f*PI * gs_torque) / (w * physics->motor.I * MOTOR_N_POLES * 2);
	r = UNIFORM_PDF_VAR (r);
	
	// Compute gain: K = P.C' / (C.P.C' + r) 
	K = gs_Pw / (gs_Pw + r);
	
	// Compute innovation
	inno = w-gs_w;	
	
	// Measurement update (P = P - K.C.P)
	gs_w += inno * K;
	gs_Pw = gs_Pw - K*gs_Pw;

	// Sanity check and recovery
	if (IsNaN (&gs_Pw) || gs_Pw < 0.0f || gs_Pw > 10000000.0f)
	{
		gs_P = 1.0f;
	}
}


// ===========================================================================
// <KFREE_GetState>
///
/// \brief	Return the estimated pendulum state
///
/// \param[out]	state	Current estimated state
// ===========================================================================
void KFREE_GetState (CTRL_State* state)
{
	state->bodyAngle = (Short) _2_DECI_DEG (gs_theta);
	state->bodySpeed = (Short) _2_DECI_DEG (gs_wz);
	state->wheelSpeed = (Short) _2_RPM (gs_w);
}



// ###########################################################################
//
// P R I V A T E
//
// ###########################################################################

// ===========================================================================
// <PredictWheelSpeedUpdate>
///
/// \brief		Update how the wheel speed evolve according to the motor torque
///				and friction model
///
/// \param		torque		Torque delivered to the motor [N.m]
/// \param		dt			Time step [s]
/// \param		brake		True if the torque results from braking
///
/// \return		Speed prediction  [rad/s]
// ===========================================================================
static float PredictWheelSpeedUpdate (float torque, float dt, Bool brake)
{
	float friction;
	float equiSpeed;
	float w, dw;
	Bool crossing;
	const PDL_Physics* p;
 
	p = PDL_GetPhysics ();
	
	// Get friction at this speed
	friction = MOTOR_GetFriction (gs_w);
	
	// Get required speed to achieve equilibrium with friction
	if (brake == False)	equiSpeed = MOTOR_GetFrictionSpeed (-torque);
	else 
	{
		equiSpeed = 0;
		torque = fabs (torque);
		if (gs_w >= 0.0f) torque = -torque;
	}
		
	// Compute speed update
	dw = (torque + friction) * dt / p->motor.I;
	
	// Detect crossing with the equilibrium point
	w = gs_w;
	crossing =  ( ((w <= equiSpeed) && ((w+dw) >= equiSpeed)) ||
				((w >= equiSpeed) && ((w+dw) <= equiSpeed)) );
	
	if (crossing) w = equiSpeed;
	else w = gs_w + dw;
	
	return w;
}

// End
