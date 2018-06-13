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
/// \file 	Matrix.h
/// \author	Jean-Sebastien Gonsette
/// \date	04/11/2014
///
// ###########################################################################

#ifndef _MATRIX_H
#define _MATRIX_H


// ===========================================================================
// I N C L U D E
// ===========================================================================

#include "NanoScale.h"



// ###########################################################################
//
// T Y P E S
//
// ###########################################################################

/// Matrix type
typedef struct
{
	float *pRaw;		///< Pointer on raw values (size=w*x)
	Word rawSize;		///< Raw buffer size
	Bool rawSwapped;	///< Raw values should be considered transposed
	Byte w;				///< Matrix width
	Byte h;				///< Matrix height
}
Matrix;



// ###########################################################################
//
// P R O T O T Y P E S
//
// ###########################################################################

void MTX_Init (Matrix *pA, float *pRaw, int rawSize, int nRows, int nColumns);
void MTX_Copy (Matrix *pB, const Matrix *pA);

void MTX_SelfNull (Matrix *pA);
void MTX_SelfTranspose (Matrix *pA);
void MTX_SelfInverse (Matrix *pA);

float MTX_Get (const Matrix *pA, int r, int c);
void MTX_Set (Matrix *pA, int r, int c, float v);

void MTX_Add (const Matrix *pA, const Matrix *pB, Matrix *pC);
void MTX_Sub (const Matrix *pA, const Matrix *pB, Matrix *pC);

void MTX_MulK (const Matrix *pA, float K, Matrix *pB);
void MTX_Mul (const Matrix *pA, const Matrix *pB, Matrix *pC);

#endif
