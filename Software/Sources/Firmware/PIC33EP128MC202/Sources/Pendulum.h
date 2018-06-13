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
/// \file 	Pendulum.h
/// \author	Jean-Sebastien Gonsette
/// \date	03/12/2014
///
// ###########################################################################

#ifndef _PENDULUM_H
#define _PENDULUM_H


// ===========================================================================
// I N C L U D E
// ===========================================================================

#include "NanoScale.h"



// ===========================================================================
// D E F I N E
// ===========================================================================

/// PI constant
#define PI		(3.14159265358979f)

/// Gravity constant [m/s^2]	
#define G		(9.81f)

/// Variance of a uniform pdf in interval [-x; x]
#define UNIFORM_PDF_VAR(x)	((float)(x)*(float)(x)/3.0f)


// ===========================================================================
// C O N V E R T I O N   T O   M K S 
// ===========================================================================

/// Convertion between [RPM] to [rad/s]
#define RPM(rpm)			((float)(rpm)*2.0f*PI/60.0f)

/// Convertion between [1/10 °] to [rad]
#define DECI_DEG(a)			((float)(a)*PI/1800.0f)

/// Convertion between [1/1000 °] to [rad]
#define MILLI_DEG(a)		((float)(a)*PI/180000.0f)

/// Convertion between [°] to [rad]
#define DEG(a)				((float)(a)*PI/180.0f)

/// Convertion between [ms] to [s]
#define MILLI_SECONDS(x)	((float)(x)*1e-3f)

/// Convertion between [mg] to [m/s^2]
#define MILLI_G(x)			((float)(x)*G*1e-3f)


// ===========================================================================
// C O N V E R T I O N   F R O M   M K S 
// ===========================================================================

/// Convertion between [rad/s] to [RPM]
#define _2_RPM(a)			((float)(a)*60.0f/(2.0f*PI))

/// Convertion between [rad] to [1/10 °] to [rad]
#define _2_DECI_DEG(a)			((float)(a)*1800.0f/PI)

/// Convertion between [rad] to [1/1000 °]
#define _2_MILLI_DEG(a)		((float)(a)*180000.0f/PI)



// ###########################################################################
//
// T Y P E S
//
// ###########################################################################

/// Motor (with reaction wheel) properties
typedef struct
{
	float m;			///< Total mass [kg]
	float I;			///< Rotor inertia [kg.m^2]
	float trqBrake;		///< Braking torque [N.m]
	float trqStall;		///< Stall torque [N.m]
	float Km;			///< Motor torque constant [N.m/A]
	float Cw;			///< Rotor friction (linear coefficient)
	int nPoles;			///< Number of poles
	int rpmMax;			///< Max rotation speed [RPM]
}
PDL_Motor;


/// Pendulum body properties
typedef struct
{
	float m;			///< Total mass [kg]
	float I;			///< Rotor inertia [kg.m^2]
	float lb;			///< Body gravity center height (from ground) [m]
	float lw;			///< Motor/Wheel gravity center height (from ground) [m]
	float Cb;			///< Body friction coefficient on ground
	float ra;			///< Distance between accelerometers and ground [m]
}
PDL_Body;


// ===========================================================================
/// Pendulum physical properties
// ===========================================================================
typedef struct
{
	PDL_Motor motor;	///< Motor properties
	PDL_Body body;		///< Body properties

	float l;			///< Pendulum gravity center height [m]
	float M;			///< Product of mass and gravity center height [kg.m]
	float I;			///< Pendulum inertia without wheel, around pivot point [kg.m^2]
}
PDL_Physics;



// ###########################################################################
//
// P R O T O T Y P E S
//
// ###########################################################################

void PDL_Init ();
const PDL_Physics* PDL_GetPhysics ();


#endif
