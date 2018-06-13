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
/// \file 	Controller.h
/// \author	Jean-Sebastien Gonsette
/// \date	12/10/2014
///
// ###########################################################################

#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include "NanoScale.h"



// ###########################################################################
//
// T Y P E S
//
// ###########################################################################

// ===========================================================================
/// Controller mode
// ===========================================================================
typedef enum
{
	CTRL_Stop,				///< Controller is stopped
	CTRL_AcqOnly,			///< Sensor acquisition only
	CTRL_Free,				///< No control, simple estimation of pendulum angle
	CTRL_Control			///< Controlling pendulum angle
}
CTRL_Mode;


// ===========================================================================
/// Sensors value
// ===========================================================================
typedef struct
{
	Word bldcSpeed;			///< Motor speed [RPM]
	Short accXYZ [3];		///< Accelerations [mg]
	Long gyroZ;				///< Angular speed [1/1000 °/s]
}
CTRL_Sensors;


// ===========================================================================
/// Pendulum estimated state
// ===========================================================================
typedef struct
{
	Short verticality;		///< Body angle around vertical [-90; 90°] [1/10 °]
							///< <0: Motor is down
							///< >0: Motor is up
	Short bodyAngle;		///< Body angle [1/10 °]
							///< 0: Equilibrium point
	Short bodySpeed;		///< Body angular speed [1/10 °/s]
	Short wheelSpeed;		///< Wheel speed [RPM]
}
CTRL_State;


// ===========================================================================
/// Controller command
// ===========================================================================
typedef struct
{
	Long torque;			///< Torque to give to the motor [uN.m]
	Bool brake;				///< Brake command to the motor ?
	Bool stop;				///< No command to the motor ?
	Bool end;				///< Controller has finished its job ?
}
CTRL_Command;



// ###########################################################################
//
// P R O T O T Y P E S
//
// ###########################################################################

void CTRL_Init ();
void CTRL_SetMode (CTRL_Mode mode);
Bool CTRL_WaitCycle (Long timeout, Dword *pCount);

void CTRL_ManualMotorCommand (int torque, Bool brake);

void CTRL_GetPendulumSensors (CTRL_Sensors* sensors);
void CTRL_GetPendulumState (CTRL_State* state);
void CTRL_GetPendulumTargetState (CTRL_State* targetState);
void CTRL_GetPendulumCommand (CTRL_Command* command);

#endif