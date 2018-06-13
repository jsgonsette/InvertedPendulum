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
/// \file	Matrix.c
/// \author	Jean-Sebastien Gonsette
/// \date	04/11/2014
///
/// Simple operations on matrices
// ###########################################################################

#include "Matrix.h"
#include "Debug.h"


// ===========================================================================
// D E F I N E
// ===========================================================================


// ===========================================================================
// T Y P E S
// ===========================================================================


// ===========================================================================
// P R I V A T E
// ===========================================================================


// ===========================================================================
// P R I V A T E   P R O T O
// ===========================================================================

/// \name Internal functions
static void Invert3 (Matrix *pA);
static Bool SameSize (const Matrix *pA, const Matrix *pB);
static float DotProduct (const Matrix *pA, int r, const Matrix *pB, int c);
//@}



// ###########################################################################
//
// P U B L I C
//
// ###########################################################################

// ===========================================================================
// <MTX_Init>
// ===========================================================================
void MTX_Init (Matrix *pA, float *pRaw, int rawSize, int nRows, int nColumns)
{
	pA->pRaw = pRaw;
	pA->rawSize = rawSize;
	pA->w = nColumns;
	pA->h = nRows;
	pA->rawSwapped = False;
}


// ===========================================================================
// <MTX_Copy>
///
/// \brief		Copy a matrix into another matrix
///
// ===========================================================================
void MTX_Copy (Matrix *pB, const Matrix *pA)
{
	int i;
	
	i = pA->w * pA->h;
	ASSERT (pB->rawSize >= i);
	
	pB->w = pA->w;
	pB->h = pA->h;
	pB->rawSwapped = pA->rawSwapped;
	while (--i >= 0) pB->pRaw [i] = pA->pRaw [i];
}


// ===========================================================================
// <MTX_Add>
///
/// \brief		Matrix addition
///
/// \param[in]	A
/// \param[in]	B
/// \param[out]	C
///
/// return C = A + B
// ===========================================================================
void MTX_Add (const Matrix *pA, const Matrix *pB, Matrix *pC)
{
	int i;
	
	i = pA->w * pA->h;

	ASSERT (SameSize (pA, pB));
	ASSERT (pC->rawSize >= i);
	
	pC->w = pA->w;
	pC->h = pA->h;
	pC->rawSwapped = pA->rawSwapped;
	
	if (pA->rawSwapped == pB->rawSwapped) 
		while (--i >= 0) pC->pRaw [i] = pA->pRaw [i]+pB->pRaw [i];
	else ASSERT (0);/// \todo
}


// ===========================================================================
// <MTX_Sub>
///
/// \brief		Matrix substraction
///
/// \param[in]	A
/// \param[in]	B
/// \param[out]	C
///
/// return C = A - B
// ===========================================================================
void MTX_Sub (const Matrix *pA, const Matrix *pB, Matrix *pC)
{
	int i;
	
	i = pA->w * pA->h;

	ASSERT (SameSize (pA, pB));
	ASSERT (pC->rawSize >= i);
	
	pC->w = pA->w;
	pC->h = pA->h;
	pC->rawSwapped = pA->rawSwapped;

	if (pA->rawSwapped == pB->rawSwapped) 
		while (--i >= 0) pC->pRaw [i] = pA->pRaw [i]-pB->pRaw [i];
	else ASSERT (0);/// \todo
}


// ===========================================================================
// <MTX_Get>
///
/// \brief	Get a matrix element
///
/// \param	pA	Target matrix
/// \param 	r	Matrix Row
/// \param 	c	Matrix Column
///
/// \return		Matrix value at (r, c)
// ===========================================================================
float MTX_Get (const Matrix *pA, int r, int c)
{
	if (pA->rawSwapped == False) 
		return pA->pRaw [r*pA->w + c];
	else
		return pA->pRaw [c*pA->h + r];
}


// ===========================================================================
// <MTX_Set>
///
/// \brief	Get a matrix element
///
/// \param	pA	Target matrix
/// \param 	r	Matrix Row
/// \param 	c	Matrix Column
// ===========================================================================
void MTX_Set (Matrix *pA, int r, int c, float v)
{
	if (pA->rawSwapped == False) 
		pA->pRaw [r*pA->w + c] = v;
	else
		pA->pRaw [c*pA->h + r] = v;
}


// ===========================================================================
// <MTX_MulK>
///
/// \brief		Matrix multiplication by a real
///
/// \param[in]	A
/// \param[in]	K
/// \param[out]	B
///
/// return B = A * K
// ===========================================================================
void MTX_MulK (const Matrix *pA, float K, Matrix *pB)
{
	int i;
	
	i = pA->w * pA->h;
	ASSERT (pB->rawSize >= i);
	
	pB->w = pA->w;
	pB->h = pA->h;
	pB->rawSwapped = pA->rawSwapped;
	while (--i >= 0) pB->pRaw [i] = pA->pRaw [i] * K;
}


// ===========================================================================
// <MTX_Mul>
///
/// \brief		Matrix multiplication
///
/// \param[in]	A
/// \param[in]	B
/// \param[out]	C
///
/// return C = A * B
// ===========================================================================
void MTX_Mul (const Matrix *pA, const Matrix *pB, Matrix *pC)
{
	int r, c, i;
	float v;
	
	ASSERT (pC->rawSize >= pA->h*pB->w);
	ASSERT (pA->w == pB->h);
	
	pC->w = pB->w;
	pC->h = pA->h;
	pC->rawSwapped = False;
	for (i = 0, r = 0; r < pC->h; r ++)
	{
		for (c = 0; c < pC->w; c ++) 
		{
			v = DotProduct (pA, r, pB, c);
			MTX_Set (pC, r, c, v);
			//pC->pRaw [i ++] = v; ///\todo test this
		}
	}
}


// ===========================================================================
// <MTX_SelfNull>
///
/// \brief	Nullify a matrix
///
/// \param pA		Target matrix
// ===========================================================================
void MTX_SelfNull (Matrix *pA)
{
	int i;
	
	i = pA->w * pA->h;
	pA->rawSwapped = False;
	while (--i >= 0) pA->pRaw [i] = 0.0f;
}


// ===========================================================================
// <MTX_SelfTranspose>
///
/// \brief	Transpose a matrix
///
/// \param pA		Target matrix
// ===========================================================================
void MTX_SelfTranspose (Matrix *pA)
{	
	// Inverse dedicated flag
	if (pA->rawSwapped == True) pA->rawSwapped = False;
	else pA->rawSwapped = True;
	
	// Swap width and height
	pA->w ^= pA->h;
	pA->h ^= pA->w;
	pA->w ^= pA->h;
}


// ===========================================================================
// <MTX_SelfInverse>
///
/// \brief	Inverse a matrix
///
/// \param pA		Target matrix
// ===========================================================================
void MTX_SelfInverse (Matrix *pA)
{	
	float det;
	float t;
	ASSERT (pA->w == pA->h);
		
	switch (pA->w)
	{
	case 1:pA->pRaw [0] = 1/pA->pRaw [0]; break;
	
	case 2:
		det = pA->pRaw [0]*pA->pRaw [3] - pA->pRaw [1]*pA->pRaw [2];
		t = pA->pRaw [0];
		pA->pRaw [0] = pA->pRaw [3] / det;
		pA->pRaw [3] = t/det;
		pA->pRaw [1] = -pA->pRaw [1]/det;
		pA->pRaw [2] = -pA->pRaw [2]/det;
	break;
	
	case 3:	Invert3 (pA); break;
	
	default: ASSERT (0); break;
	}
}



// ###########################################################################
//
// P R I V A T E
//
// ###########################################################################

// ===========================================================================
// <Invert3>
///
/// \brief	3 by 3 matrix inversion
///
/// \param 	pA	3x3 matrix to invert
// ===========================================================================
static void Invert3 (Matrix *pA)
{
	float a, b, c, d, e, f, g, h, i;
	float det;
	
	a = pA->pRaw [0];
	b = pA->pRaw [1];
	c = pA->pRaw [2];
	d = pA->pRaw [3];
	e = pA->pRaw [4];
	f = pA->pRaw [5];
	g = pA->pRaw [6];
	h = pA->pRaw [7];
	i = pA->pRaw [8];
	
	det = a*(e*i-h*f) + c*(d*h-g*e) + b*(g*f-d*i);
	pA->pRaw [0] = (e*i-h*f)/det;	
	pA->pRaw [1] = (h*c-b*i)/det;	
	pA->pRaw [2] = (b*f-e*c)/det;	

	pA->pRaw [3] = (g*f-d*i)/det;	
	pA->pRaw [4] = (a*i-g*c)/det;	
	pA->pRaw [5] = (d*c-a*f)/det;	
	
	pA->pRaw [6] = (d*h-g*e)/det;	
	pA->pRaw [7] = (g*b-a*h)/det;	
	pA->pRaw [8] = (a*e-b*d)/det;	
}


// ===========================================================================
// <SameSize>
///
/// \brief	Check 2 matrices have same size
///
/// \param 	pA	First matrix
/// \param 	pB	Second matrix
///
/// \return 	True if A and B have same size
// ===========================================================================
static Bool SameSize (const Matrix *pA, const Matrix *pB)
{
	if (pA->w == pB->w && pA->h == pB->h) return True;
	return False;
}


// ===========================================================================
// <DotProduct>
///
/// \brief	Compute the dot product between A matrix row and B matrix column
///
/// \param	pA		A matrix to consider
/// \param	r		Row of A matrix
/// \param	pB		B matrix to consider
/// \param	c		Column of B matrix
///
/// \return A[r,:] dot B [:,c]
// ===========================================================================
static float DotProduct (const Matrix *pA, int r, const Matrix *pB, int c)
{
	int i;
	float *pa, *pb;
	float v = 0.0f;
	int stepA, stepB;
	
	// Point to the row head in A
	if (pA->rawSwapped == False)
	{
		pa = pA->pRaw + r*pA->w;
		stepA = 1;
	}
	else
	{
		pa = pA->pRaw + r;
		stepA = pA->h;
	}
	
	// Point to the column head in B
	if (pB->rawSwapped == False)
	{
		pb = pB->pRaw + c;
		stepB = pB->w;
	}
	else
	{
		pb = pB->pRaw + c*pB->h;
		stepB = 1;
	}
	
	// Dot product loop
	for (i = 0; i < pA->w; i ++)
	{
		v += (*pa) * (*pb);
		pa += stepA;
		pb += stepB;
	}
	
	return v;
}

// End
