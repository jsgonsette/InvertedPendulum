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
/// \file	Pendulum.c
/// \author	Jean-Sebastien Gonsette
/// \date	03/12/2014
///
/// This modules handles all the physical properties of the pendulum 
/// and of its sensors.
// ###########################################################################

#include "Pendulum.h"


// ===========================================================================
// D E F I N E
// ===========================================================================

#define MB			0.138f		///< Pendulum body mass [kg]
#define MW			0.094f		///< Pendulum wheel mass [kg]
#define LB			0.0430f		///< Distance between ground and body center of mass [m]
#define LW			0.0406f		///< Distance between ground and wheel center of mass [m]
#define IB			9.85e-5f	///< Pendulum body inertia [kg.m^2]
#define IW			4.20e-5f	///< Pendulum wheel inertia [kg.m^2]
#define RA			0.015f		///< Distance between ground and accelerometer device [m]
#define TRQ_B		0.079f		///< Braking torque [N.m]
#define KM			25.1e-3f	///< Motor torque constant [N.m/A]


// ===========================================================================
// T Y P E S
// ===========================================================================


// ===========================================================================
// P R I V A T E
// ===========================================================================

static PDL_Physics gs_physics;


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
// <PDL_Init>
///
/// \brief	Init all the pendulum properties
// ===========================================================================
void PDL_Init ()
{
	gs_physics.motor.m = MW;
	gs_physics.motor.I = IW;
	gs_physics.motor.trqBrake = TRQ_B;
	gs_physics.motor.trqStall = 0;
	gs_physics.motor.Km = KM;
	gs_physics.motor.Cw = 1.85e-5f;
	gs_physics.motor.nPoles = 8;
	gs_physics.motor.rpmMax = 4000;

	gs_physics.body.m = MB;
	gs_physics.body.I = IB;
	gs_physics.body.lb = LB;
	gs_physics.body.lw = LW;
	gs_physics.body.Cb = 2.00e-6f;
	gs_physics.body.ra = RA;
							
	gs_physics.l = (gs_physics.motor.m * gs_physics.body.lw + gs_physics.body.m * gs_physics.body.lb) / 
					(gs_physics.motor.m + gs_physics.body.m);
					
	gs_physics.M = gs_physics.body.m * gs_physics.body.lb + 
					gs_physics.motor.m * gs_physics.body.lw;

	gs_physics.I = gs_physics.body.I +
					gs_physics.body.m * gs_physics.body.lb * gs_physics.body.lb + 
					gs_physics.motor.m * gs_physics.body.lw * gs_physics.body.lw;
}


// ===========================================================================
// <PDL_GetPhysics>
///
/// \brief	Get pendulum physical properties
// ===========================================================================
const PDL_Physics* PDL_GetPhysics ()
{
	return &gs_physics;
}


// End
