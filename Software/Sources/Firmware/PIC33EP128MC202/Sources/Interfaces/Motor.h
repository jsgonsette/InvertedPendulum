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
/// \file 	Motor.h
/// \author	Jean-Sebastien Gonsette
/// \date	20/10/2014
///
// ###########################################################################

#ifndef _MOTOR_H
#define _MOTOR_H

// ===========================================================================
// I N C L U D E
// ===========================================================================
#include "NanoScale.h"


// ===========================================================================
// D E F I N E
// ===========================================================================

/// Number of poles of the motor
#define MOTOR_N_POLES		8

/// Stall torque [mN.m]
#define MOTOR_STALL_TORQUE	255

/// No load speed [RPM]
#define MOTOR_FULL_SPEED	4370

/// Max torque limit [mN.m]
#define MOTOR_MAX_TORQUE_LIMIT	50



// ###########################################################################
//
// T Y P E S
//
// ###########################################################################

// ===========================================================================
/// Motor mode
// ===========================================================================
typedef enum
{
	MOTOR_Disabled = 0,
	MOTOR_Brake,
	MOTOR_Forward,
	MOTOR_Reverse
}
MOTOR_Mode;


// ===========================================================================
// Double regression coefficients for torque calibration inside one quadrant
// ===========================================================================
typedef struct
{
	float t0_0;		///< Calibration law static coefficient, static [N.m]
	float t0_t;		///< Calibration law static coefficient, input torque dependant [N.m/N.m]
	float tw_0;		///< Calibration law speed dependant coefficient, static [N.m/(rad/s)]
	float tw_t;		///< Calibration law speed dependant coefficient, input torque dependant [N.m/(N.m)(rad/s)]
}
MOTOR_QuadrantCalib;


// ===========================================================================
/// Motor calibration values
// ===========================================================================
typedef struct
{
	float friction_f0;				///< Static friction coefficient [N.m]
	float friction_f1;				///< Linear friction coefficient [N.m/(rad/s)]

	MOTOR_QuadrantCalib calib_low [4];	///< 4 Quadrants calibration for low torque
	MOTOR_QuadrantCalib calib_high [4];	///< 4 Quadrants calibration for high torque
}
MOTOR_CalibValues;



// ###########################################################################
//
// P R O T O T Y P E S
//
// ###########################################################################

void MOTOR_Init (float Iw);

float MOTOR_GetBrakingTorque (float w);
float MOTOR_GetFriction (float w);
float MOTOR_GetFrictionSpeed (float torque);

Long MOTOR_SetTorque (Long torque, Short motorSpeed);
void MOTOR_RawCommand (MOTOR_Mode mode, Word threshold);

void MOTOR_GetCalibrationValues (MOTOR_CalibValues *calib);
void MOTOR_SetCalibrationValues (MOTOR_CalibValues calib);

#endif
