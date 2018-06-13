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
/// \file	Motor.c
/// \author	Jean-Sebastien Gonsette
/// \date	20/10/2014
///
/// This module provides interface to drive the BLDC motor.
// ###########################################################################

#include <math.h>
#include "Debug.h"
#include "BSP.h"
#include "Global.h"
#include "Motor.h"
#include "Pendulum.h"
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

/// Calibration values
static MOTOR_CalibValues gs_calib;

/// Stall torque
static float gs_tStall;

/// No load max. speed
static float gs_maxSpeed;

/// Max. torque limit settable to the BLDC controller
static float gs_maxTorqueLimit;

/// Rotor total inertia
static float gs_Iw;


// ===========================================================================
// P R I V A T E   P R O T O
// ===========================================================================

/// \name Internal functions
//@{
static void SetTorqueFromThrottle (float torque, float speed, float inertia, float dt);
static void SetTorqueFromCurrentLimit (float torque, float speed);
static float CalibrateTorqueRequest (float torque, float speed, Bool highMode);
static void RawCommand (MOTOR_Mode mode, Word threshold);
static void SetThrottle (Short throttle);
//@}



// ###########################################################################
//
// P U B L I C
//
// ###########################################################################

// ===========================================================================
// <MOTOR_Init>
///
/// \brief		Initialize the motor properties
///
/// Rotor inertia is used by this module to control motor torque on the basis
/// of speed increment.
///
/// \param	Iw		Rotor total intertia (with load)  [K/m^2]
// ===========================================================================
void MOTOR_Init (float Iw)
{
	// Default, uncalibrated friction curve
	gs_calib.friction_f0 = 0.001f;
	gs_calib.friction_f1 = 9.6e-6f;
	
	gs_calib.calib_high [0].t0_0 = 0.00171f;
	gs_calib.calib_high [0].t0_t = 0.777f;
	gs_calib.calib_high [0].tw_0 = -1.82e-5f;
	gs_calib.calib_high [0].tw_t = 9.97e-4f;

	gs_calib.calib_high [1].t0_0 = 0.00187f;
	gs_calib.calib_high [1].t0_t = 0.709f;
	gs_calib.calib_high [1].tw_0 = 6.89e-6f;
	gs_calib.calib_high [1].tw_t = -0.000686f;

	gs_calib.calib_high [2].t0_0 = -0.00119f;
	gs_calib.calib_high [2].t0_t = 0.799f;
	gs_calib.calib_high [2].tw_0 = -1.598e-5f;
	gs_calib.calib_high [2].tw_t = -0.000885f;

	gs_calib.calib_high [3].t0_0 = 0.00119f;
	gs_calib.calib_high [3].t0_t = 0.905f;
	gs_calib.calib_high [3].tw_0 = -7.98e-6f;
	gs_calib.calib_high [3].tw_t = -0.000393f;

	gs_calib.calib_low [0].t0_0 = 0.00068f;
	gs_calib.calib_low [0].t0_t = 0.5839f;
	gs_calib.calib_low [0].tw_0 = -2.887e-6f;
	gs_calib.calib_low [0].tw_t = 0.00134f;

	gs_calib.calib_low [1].t0_0 = -5.547e-5f;
	gs_calib.calib_low [1].t0_t = 0.795f;
	gs_calib.calib_low [1].tw_0 = 1.131e-5f;
	gs_calib.calib_low [1].tw_t = 0.00156f;

	gs_calib.calib_low [2].t0_0 = -0.000403f;
	gs_calib.calib_low [2].t0_t = 0.67008f;
	gs_calib.calib_low [2].tw_0 = 2.63e-6f;
	gs_calib.calib_low [2].tw_t = -5.11e-5f;

	gs_calib.calib_low [3].t0_0 = -3.189e-6f;
	gs_calib.calib_low [3].t0_t = 0.7937f;
	gs_calib.calib_low [3].tw_0 = 1.059e-5f;
	gs_calib.calib_low [3].tw_t = -0.00162f;

	gs_tStall = (float) MOTOR_STALL_TORQUE / 1000.0f;
	gs_maxSpeed = RPM (MOTOR_FULL_SPEED);
	gs_maxTorqueLimit = (float) MOTOR_MAX_TORQUE_LIMIT / 1000.0f;
	
	gs_Iw = Iw;
}


// ===========================================================================
// <MOTOR_GetFriction>
///
/// \brief	Return the motor friction torque for a given rotation speed.
///
/// \param 	w	Rotor angular speed [rad/s]
///
/// \return	Friction torque [N.m].
///			<0 when wheel goes forward.
///			>0 when wheel goes backward
// ===========================================================================
float MOTOR_GetFriction (float w)
{
	float f;

	// Negative friction if speed is positive
	if (w >= 0.0f) f = -(gs_calib.friction_f1*w + gs_calib.friction_f0);
	
	// Positive friction in the other case
	else f = -(gs_calib.friction_f1*w - gs_calib.friction_f0);
	
	return f;
}


// ===========================================================================
// <MOTOR_GetBrakingTorque>
///
/// \brief	Return the motor braking torque for a given rotation speed.
///
/// \param 	w	Rotor angular speed [rad/s]
///
/// \return	Braking torque [N.m].
///			<0 when wheel goes forward.
///			>0 when wheel goes backward
// ===========================================================================
float MOTOR_GetBrakingTorque (float w)
{
	Bool neg;
	float bTrq;

	// Torque sign depends on speed direction
	neg = (w <= 0.0f ? True : False);
	if (w <= 0.0f) w = -w;
	
	// Get torque from speed
	bTrq = gs_tStall * w / gs_maxSpeed;
	if (bTrq > 0.100f) bTrq = 0.100f;
	
	// Set torque sign
	if (neg == False) bTrq = -bTrq;
	
	return bTrq;
}


// ===========================================================================
// <MOTOR_GetFriction>
///
/// \brief	Return the motor speed needed to achieve a given friction torque
///
/// \param 	torque	Friction torque [N.m]
///
/// \return	Motor speed [rad/s].
///			>0 when wheel goes forward (negative torque).
///			<0 when wheel goes backward (positive torque)
// ===========================================================================
float MOTOR_GetFrictionSpeed (float torque)
{
	float w;
	Bool neg = False;
	
	if (torque < 0.0f) 
	{
		torque = -torque;
		neg = True;
	}
	
	// Null speed if torque below stall friction
	if (torque <= gs_calib.friction_f0) return 0.0f;
	
	// Inverse friction law
	torque -= gs_calib.friction_f0;
	w = -torque / gs_calib.friction_f1;
	
	if (neg) w = -w;
	return w;
}


// ===========================================================================
// <MOTOR_SetTorque>
///
/// \brief	Give a torque command to the motor.
///
/// Positive torque value make the rotor turn forward, while negative value make it turn backward.
///
/// \param 	torque		Torque to deliver by the motor [µN.m]
/// \param 	motorSpeed	Current motor speed [RPM]
///
/// \return	Actual torque that can be delivered by the motor [µN.m]
// ===========================================================================
Long MOTOR_SetTorque (Long torque, Short motorSpeed)
{
	float t;
	float w;
	float tMax, tMin;

	// Convert to MKS
	w = RPM (motorSpeed);
	t = torque*1e-6f;

	// Torque truncation according to motor law 
	tMax = +gs_tStall * (1.0f - w / gs_maxSpeed);
	tMin = -gs_tStall * (1.0f + w / gs_maxSpeed);
	if (t > tMax) t = tMax;
	if (t < tMin) t = tMin;

	// Torque truncation according to motor controller
	if (t > gs_maxTorqueLimit) t = gs_maxTorqueLimit;
	if (t < -gs_maxTorqueLimit) t = -gs_maxTorqueLimit;
	
	// Low torque method (open loop control based on speed)
	if ((t <= 0.005f && t >= -0.005f) || (motorSpeed > -500 && motorSpeed < 500))
	{
		t = CalibrateTorqueRequest (t, w, False);
		SetTorqueFromThrottle (t, w, gs_Iw, 0.01);
	}
	
	// High torque method (based on current limit)
	else 
	{
		t = CalibrateTorqueRequest (t, w, True);
		SetTorqueFromCurrentLimit (t, w);
	}

	return (Long)(1e6f * t);
}


// ===========================================================================
// <MOTOR_RawCommand>
///
/// \brief		Give a raw command to the motor with a maximum current threshold.
///
/// Torque/current is a given as a number between 0 and 1 and is not calibrated.
/// This parameter gives an image of the current (and thus torque) that the
/// motor controller will try to regulate in the motor.
///
/// When disabled, no torque is applied on the motor.
/// When braking, an electromagnetic torque of maximum 100mN (5A) is applied on the motor
///
/// \param	mode		'Disable', 'brake', 'Forward' or 'Reverse'
/// \param 	threshold	[0..1000 °/°°] Image of the maximum torque/current to drive the motor
///						Valid for 'forward' and 'reverse' mode only
// ===========================================================================
void MOTOR_RawCommand (MOTOR_Mode mode, Word threshold)
{
	RawCommand (mode, threshold);
}


// ===========================================================================
// <MOTOR_GetCalibrationValues>
///
/// \brief	Get calibration values for backup purpose
///
/// \param[out]		calib				Calibration values
// ===========================================================================
void MOTOR_GetCalibrationValues (MOTOR_CalibValues *calib)
{
}


// ===========================================================================
// <MOTOR_SetCalibrationValues>
///
/// \brief	Set/restore calibration values.
///
/// \param[in]		calib				Calibration values
// ===========================================================================
void MOTOR_SetCalibrationValues (MOTOR_CalibValues calib)
{
}



// ###########################################################################
//
// P R I V A T E
//
// ###########################################################################

// ===========================================================================
// <SetTorqueFromThrottle>
///
/// \brief	Prepare a torque command to the motor by relying on the torque vs
///			speed law. This law depends on the speed and on the DC motor voltage.
///
/// Target torque is achieved by modulating the voltage applied to the motor
/// (by applying a PWM signal on the forward/reverse pin)
///
/// Advantage: Full 4 quadrant controlling capability
/// Drawbacks: Open loop control. In addition, torque is higly sensitive to speed.
///				This means that the best we can do is to reach a mean
///				target torque over a given timestep.
///
/// \param		torque	Target torque to deliver [N.m]
/// \param		speed	Current motor speed [rad/s]
/// \param		inertia	Total rotor inertia [kg.m^2]
/// \param		dt		Timestep during which we expect a mean torque [s]
// ===========================================================================
static void SetTorqueFromThrottle (float torque, float speed, float inertia, float dt)
{
	float throttle;
	float friction;
	float e, k;
	float w0, wf, dw;
	Short command;
	Bool zeroCrossing;
	Bool smallTorque;
	
	// Compute expected speed increment
	friction = MOTOR_GetFriction (speed);
	dw = (torque+friction) * dt / inertia;
	
	w0 = speed;
	wf = speed + dw;
	
	// Detect case where torque is below static friction
	zeroCrossing = (w0 <= 0 && wf >= 0) || (w0 >= 0 && wf <= 0);
	smallTorque = (torque < gs_calib.friction_f0 && torque > -gs_calib.friction_f0);
	
	if (zeroCrossing && smallTorque)
	{
		RawCommand (MOTOR_Disabled, 0);
		return ;
	}
	
	// Compute initial torque value
	k = gs_tStall / gs_maxSpeed;
	e = FT_Exp (-k*dt/inertia);
	
	torque = k * (w0*e - wf) / (e-1);
	
	// Compute corresponding throttle
	throttle = (torque - friction) / gs_tStall;
	ASSERT (throttle <= 1.0f);
	ASSERT (throttle >= -1.0f);
	
	// Compute the command level
	command = (Short) (1000.0f * throttle);
	
	// Set the command
	RawCommand (MOTOR_Forward, 1000);
	SetThrottle (command);
}

	
// ===========================================================================
// <SetTorqueFromCurrentLimit>
///
/// \brief	Prepare a torque command to the motor by relying on the regulation 
///			capability of the BLDC controller.
///
/// Advantage: the controller does closed loop regulation
/// Drawbacks: does not work in 4 quadrants mode. Low torques are not achievable.
///
/// Two different strategies are used depending if we are accelerting the motor
/// or decelerating it.
///
/// For acceleration: the torque command is calibrated and translated into a current reference command
/// For deceleration: same than for acceleration but the throttle is adjusted
///
/// \param		torque	Target torque to deliver [N.m]
/// \param		speed	Current motor speed [rad/s]
// ===========================================================================
static void SetTorqueFromCurrentLimit (float torque, float speed)
{
	Short acc;
	Short command;
	Bool neg;
	
	// Acceleration or decceleration ?
	acc = (torque*speed >= 0.0f ? 1 : -1);
	
	// Compute command level for quadrant 1 and 3 (absolute speed is increased)
	if (acc == 1)
	{
		neg = (speed < 0);
		
		// Work with positive torque and speed
		if (torque < 0) torque = -torque;
		if (speed < 0) speed = -speed;

		// Translate to command
		command = (Short) (1000.0f * torque / gs_maxTorqueLimit);
		
		if (neg == False) RawCommand (MOTOR_Forward, command);
		else RawCommand (MOTOR_Reverse, command);
	}
	
	// Compute command level for quadrant 2 and 4 (absolute speed is decreased)
	else
	{
		// Work with positive torque
		if (torque < 0) torque = -torque;

		// Translate to command
		command = (Short) (1000.0f * torque / gs_maxTorqueLimit);
		RawCommand (MOTOR_Forward, command);
		
		// Adjust throtthle
		command /= 5;
		if (speed > 0) command = -command;
		command = 0;
		SetThrottle (command);
	}
}


// ===========================================================================
// <CalibrateTorqueRequest>
///
/// \brief	Calibrate requested torque to compensate for torque decreasing with speed
///
/// \param		torque		Target torque to deliver [N.m]
/// \param		speed		Current motor speed [rad/s]
/// \param		highMode	True for high torque calibration
///
/// \return		Calibrated torque request
// ===========================================================================
static float CalibrateTorqueRequest (float torque, float speed, Bool highMode)
{
	MOTOR_QuadrantCalib *pCalib;
	
	// Get the right calib values set
	if (highMode) pCalib = gs_calib.calib_high;
	else pCalib = gs_calib.calib_low;
	
	// Get calibration values for the quadrant we are in
	// Q1: trq +, speed +
	// Q2: trq +, speed -
	// Q3: trq -, speed -
	// Q4: trq -, speed +
	if (torque >= 0.0f)
	{
		if (speed >= 0.0f) ;
		else pCalib = pCalib +1;
	}
	else
	{
		if (speed >= 0.0f) pCalib = pCalib +3;
		else pCalib = pCalib +2;
	}
	
	// Get the torque to request to actually get the torque we need
	torque = (torque - (pCalib->t0_0 + pCalib->tw_0 * speed)) / 
				(pCalib->t0_t + pCalib->tw_t * speed);
				
	return torque;
}


// ===========================================================================
// <RawCommand>
///
/// \brief		Give a raw command to the motor with a maximum current threshold.
///
/// Torque/current is a given as a number between 0 and 1 and is not calibrated.
/// This parameter gives an image of the current (and thus torque) that the
/// motor controller will try to regulate in the motor.
///
/// When disabled, no torque is applied on the motor.
/// When braking, an electromagnetic torque of maximum 100mN (5A) is applied on the motor
///
/// \param	mode		'Disable', 'brake', 'Forward' or 'Reverse'
/// \param 	threshold	[0..1000 °/°°] Image of the maximum torque/current to drive the motor
///						Valid for 'forward' and 'reverse' mode only
// ===========================================================================
static void RawCommand (MOTOR_Mode mode, Word threshold)
{
	Dword t;

	// To get no torque, controller is disabled
	if (threshold < 10 && mode != MOTOR_Brake) mode = MOTOR_Disabled;
	if (threshold > 1000) threshold = 1000;

	switch (mode)
	{
	default:
	case MOTOR_Disabled:
		BSP_BLDCSetEnable (0);		
		BSP_BLDCSetBrake (0);
		BSP_BLDCSetRef (0);
		break;

	case MOTOR_Brake:
		BSP_BLDCSetEnable (0);

		BSP_BLDCSetForward (1);
		BSP_BLDCSetBrake (1);
		BSP_BLDCSetRef (0);

		BSP_BLDCSetEnable (1);
		break;

	case MOTOR_Forward:
	case MOTOR_Reverse:
		BSP_BLDCSetEnable (0);

		t = ((Dword) 0xFFFF * threshold)/1000;
		BSP_BLDCSetRef (t);
		BSP_BLDCSetForward (mode == MOTOR_Forward ? 1:0);
		BSP_BLDCSetBrake (0);

		BSP_BLDCSetEnable (1);
		break;
	}
}


// ===========================================================================
// <SetThrottle>
///
/// \brief		Drive the motor with a PWM throttle
///
/// This parameter defines how the motor voltage will be modulated by a PWM signal.
/// Its effect is to make motor voltage U varying between [-U; U].
///
/// \param 	throttle	[-1000..1000 °/°°] Throttle level
// ===========================================================================
static void SetThrottle (Short throttle)
{
	Short duty;

	if (throttle < -1000) throttle = -1000;
	if (throttle > 1000) throttle = 1000;

	duty = (Short) (((Long) throttle*0x7FFFL)/1000L);

	BSP_BLDCSetEnable (0);
	BSP_BLDCSetForwardDuty (duty);
	BSP_BLDCSetBrake (0);
	BSP_BLDCSetEnable (1);
}

// End
