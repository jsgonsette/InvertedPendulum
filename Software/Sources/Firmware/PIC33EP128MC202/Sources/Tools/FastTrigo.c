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
/// \file	FastTrigo.c
/// \author	Jean-Sebastien Gonsette
/// \date	10/11/2014
///
/// This module provides fast, less accurate trigonometric operation
// ###########################################################################

#include <math.h>
#include "FastTrigo.h"


// ===========================================================================
// D E F I N E
// ===========================================================================


// ===========================================================================
// T Y P E S
// ===========================================================================


// ===========================================================================
// P R I V A T E
// ===========================================================================

static const float pi = 3.14159265358979f;


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
// <FT_Sin>
// 
/// \brief Bhaskara sinus approximation
///
/// Sinus approximation with a polynomial division:
///
/// \param	x	Angle in radian
///
/// \return sin(x)
// ===========================================================================
float FT_Sin (float x)
{
	int neg = 0;
	float s;
	
	while (x < 0.0f) x += 2.0f*pi;
	while (x > 2.0f*pi) x-= 2.0f*pi;
	
	// Approximation works only between 0 and pi
	if (x > pi)
	{
		neg = 1;
		x -= pi;
	}
	
	// Compute rational approximation
	s = (16.0f*x*(pi-x))/(49.35f-4.0f*x*(pi-x));
	
	if (neg == 0) return s;
	else return -s;
}


// ===========================================================================
// <FT_Cos>
// 
/// \brief Bhaskara cosinus approximation
///
/// \param	x	Angle in radian
///
/// \return cos(x)
// ===========================================================================
float FT_Cos (float x)
{
	// cos (x) = sin (pi/2-x)
	return FT_Sin (pi/2.0f-x);
}


// ===========================================================================
// <FT_Atan2>
//
/// \brief	Tangent inverse trigonometric operation
///
/// Arc tangent approximation with a polynomial division:
/// angle = pi/2 * (t^2 + b*t)/(t^2 + 2*b*t + 1)
/// with b = 0.596227
/// Maximal error is 0.16°
///
/// \param	y	sin(angle)
/// \param	x	cos(angle)
///
/// \return	Angle [0..2*pi][rad]
// ===========================================================================
float FT_Atan2 (float y, float x)
{
	int sx = 0, sy = 0;
	float bxy;
	
	// Which quadrant ?
	if (y < 0.0f) sy = 1;
	if (x < 0.0f) sx = 1;
	
	// Compute angle as for first quadrant
	bxy = 0.596227f * y * x;
	if (bxy < 0.0f) bxy = -bxy;
	y *= y;
	x = (y + bxy)/(y + 2.0f*bxy + x*x);
	x *= pi/2.0f;
	
	// Transpose to the right quadrant
	if (sx != sy) x = pi-x;
	if (sy == 1) x += pi;
	
	return x;
}


// ===========================================================================
// <FT_Exp>
//
/// \brief	Exponential operation
///
/// Approximation is given by e^x = (1+x/256)^256
///
/// \param	x		Input number
///
/// \return	e^x
// ===========================================================================
float FT_Exp (float x)
{
	x /= 256.0f;
	x += 1.0f;
	if (x < 0.0f) return 0.0f;

	x *= x;	x *= x;
	x *= x;	x *= x;
	x *= x;	x *= x;
	x *= x;	x *= x;
	return x;
}


// ===========================================================================
// <FT_Sqrt>
//
/// \brief	Square root approximation
///
/// It uses Newton iteration on the basis of an initial guess
///
/// \param	x		Input number
///
/// \return	x^1/2
// ===========================================================================
float FT_Sqrt (float x)
{
	float r;
	float xHalf;
	int exp;
	
	xHalf = x/2.0f;
	
	// Initial guess (divide exponent by 2)
	x = frexpf (x, &exp);
	r = ldexpf (x, exp/2);
	
	// Newton iteration
	r = r/2.0f + xHalf/r;
	return r;
}


// ===========================================================================
// <IsNaN>
//
/// \brief	Check if a float has the special value "Not a Number
///
/// \param	x		Input number
///
/// \return	True if it is NaN
// ===========================================================================
Bool IsNaN (const float* f)
{
	const int* rep = ((const int*) f) + 1;
	return ((*rep & 0x7F00) == 0x7F00);
}

// End
